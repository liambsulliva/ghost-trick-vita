#include <stdio.h>
#include <string.h>
#include "audio_volume.h"
#include "file.h"
#include "file_path.h"
#include "game_settings.h"
#include "memory.h"
#include "music_engine.h"
#include "run_log.h"
#include "text_lines.h"

#define LINE_MAX 64
#define DARKNESS_PER_STEP 0.06f

static struct game_settings current;
static int default_music = GAME_MUSIC_ARRANGED;
static char music_folders[2][FILE_PATH_MAX];

static int clamp_level(int value, int least)
{
    return value < least ? least : (value > GAME_SETTING_STEPS ? GAME_SETTING_STEPS : value);
}

static void keep_in_range(struct game_settings *settings)
{
    settings->brightness = clamp_level(settings->brightness, 1);
    settings->master = clamp_level(settings->master, 0);
    settings->music = clamp_level(settings->music, 0);
    settings->effects = clamp_level(settings->effects, 0);
    settings->music_type = settings->music_type == GAME_MUSIC_ORIGINAL ? GAME_MUSIC_ORIGINAL : GAME_MUSIC_ARRANGED;
    settings->language = 0;
    settings->unlock_all = settings->unlock_all != 0;
}

void game_settings_set_default_music(int music_type)
{
    default_music = music_type;
}

void game_settings_set_music_folders(const char *original_folder, const char *arranged_folder)
{
    snprintf(music_folders[GAME_MUSIC_ORIGINAL], sizeof(music_folders[0]), "%s", original_folder);
    snprintf(music_folders[GAME_MUSIC_ARRANGED], sizeof(music_folders[1]), "%s", arranged_folder);
}

void game_settings_defaults(struct game_settings *settings)
{
    settings->brightness = GAME_SETTING_STEPS;
    settings->master = GAME_SETTING_STEPS;
    settings->music = GAME_SETTING_STEPS;
    settings->effects = GAME_SETTING_STEPS;
    settings->music_type = default_music;
    settings->language = 0;
    settings->unlock_all = 0;
}

int game_settings_read_text(struct game_settings *settings, const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    char name[32];
    unsigned int offset;
    int value;
    int known;

    known = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (sscanf(line, "%31s %d", name, &value) != 2) {
            continue;
        }
        known = known + 1;
        if (strcmp(name, "brightness") == 0) {
            settings->brightness = value;
        } else if (strcmp(name, "master") == 0) {
            settings->master = value;
        } else if (strcmp(name, "music") == 0) {
            settings->music = value;
        } else if (strcmp(name, "effects") == 0) {
            settings->effects = value;
        } else if (strcmp(name, "music_type") == 0) {
            settings->music_type = value;
        } else if (strcmp(name, "language") == 0) {
            settings->language = value;
        } else if (strcmp(name, "unlock_all") == 0) {
            settings->unlock_all = value;
        } else {
            known = known - 1;
        }
    }
    keep_in_range(settings);
    return known;
}

unsigned int game_settings_write_text(const struct game_settings *settings, char *text, unsigned int text_size)
{
    int written;

    written = snprintf(text, text_size, "brightness %d\nmaster %d\nmusic %d\neffects %d\nmusic_type %d\nlanguage %d\nunlock_all %d\n", settings->brightness, settings->master, settings->music, settings->effects, settings->music_type, settings->language, settings->unlock_all);
    return written < 0 || (unsigned int)written >= text_size ? 0u : (unsigned int)written;
}

struct game_settings *game_settings_now(void)
{
    return &current;
}

void game_settings_load(const char *path)
{
    unsigned char *bytes;
    unsigned int size;
    int known;

    game_settings_defaults(&current);
    known = 0;
    if (file_read_whole(path, &bytes, &size) == FILE_OK) {
        known = game_settings_read_text(&current, (const char *)bytes, size);
        mem_free(bytes);
    }
    run_log_write("settings read=%d brightness=%d master=%d music=%d effects=%d music_type=%d", known, current.brightness, current.master, current.music, current.effects, current.music_type);
}

int game_settings_save(const char *path)
{
    char text[GAME_SETTINGS_TEXT_MAX];
    unsigned int size;
    int result;

    size = game_settings_write_text(&current, text, sizeof(text));
    result = size > 0 && file_write_whole(path, (const unsigned char *)text, size) == FILE_OK;
    run_log_write("settings saved=%d", result);
    return result;
}

int game_settings_volume_gain(int level)
{
    level = clamp_level(level, 0);
    return AUDIO_VOLUME_FULL * level * level / (GAME_SETTING_STEPS * GAME_SETTING_STEPS);
}

void game_settings_apply(void)
{
    int master;

    keep_in_range(&current);
    master = game_settings_volume_gain(current.master);
    audio_volume_set(AUDIO_PORT_MUSIC, master * game_settings_volume_gain(current.music) / AUDIO_VOLUME_FULL);
    audio_volume_set(AUDIO_PORT_SOUND, master * game_settings_volume_gain(current.effects) / AUDIO_VOLUME_FULL);
    if (music_folders[current.music_type][0] != '\0') {
        music_engine_use_folder(music_folders[current.music_type]);
    }
}

float game_settings_darkness(void)
{
    return (float)(GAME_SETTING_STEPS - clamp_level(current.brightness, 1)) * DARKNESS_PER_STEP;
}
