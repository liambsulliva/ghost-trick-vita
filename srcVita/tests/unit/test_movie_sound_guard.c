#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_texture_files.h"
#include "run_log.h"
#include "script_bridge.h"
#include "script_outputs.h"
#include "script_tables.h"
#include "texture_cache.h"
#include "test_support.h"

#define MOVIE_SOUND 82

static struct script_tables tables;

static int sounds_in_log(void)
{
    unsigned int size;
    const char *text;
    const char *found;
    int count;

    run_log_flush();
    text = (const char *)fake_files_contents("run.log", &size);
    count = 0;
    found = text != NULL ? strstr(text, "kind=sound name=movie_whoosh") : NULL;
    while (found != NULL) {
        count = count + 1;
        found = strstr(found + 1, "kind=sound name=movie_whoosh");
    }
    return count;
}

static const struct script_outputs *start_with_a_class_movie(void)
{
    static const char effects[] = "movie short_jump 20 3 034157e8 82\n";
    static const char audio[] = "sound 82 movie_whoosh\n";

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    run_log_open("run.log");
    texture_cache_init("tex", 1u << 20);
    script_tables_clear(&tables);
    CHECK(script_tables_read_effects(&tables, effects, (unsigned int)strlen(effects)));
    CHECK(script_tables_read_audio(&tables, audio, (unsigned int)strlen(audio)));
    script_bridge_start(&tables);
    return script_bridge_outputs();
}

static void test_the_same_tick_duplicate_waits_for_the_frames(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_a_class_movie();
    CHECK(outputs->movie_of_class_started(0x034157e8u) > 0);
    outputs->sound_played(MOVIE_SOUND);
    script_bridge_finish_tick();
    CHECK_INT(sounds_in_log(), 0);
}

static void test_a_later_sound_of_that_kind_still_plays(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_a_class_movie();
    CHECK(outputs->movie_of_class_started(0x034157e8u) > 0);
    script_bridge_finish_tick();
    outputs->sound_played(MOVIE_SOUND);
    CHECK_INT(sounds_in_log(), 1);
}

int main(void)
{
    test_the_same_tick_duplicate_waits_for_the_frames();
    test_a_later_sound_of_that_kind_still_plays();
    fake_files_reset();
    return test_finish("unit/movie_sound_guard");
}
