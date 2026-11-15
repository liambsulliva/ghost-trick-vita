#include "asset_sets.h"

#include <stdio.h>

#include "file_path.h"
#include "game_settings.h"
#include "memory.h"
#include "run_log.h"

static char chosen_folder[FILE_PATH_MAX];
static int running_filter;

static int folder_has_a_set(const char *folder)
{
    char path[FILE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;

    if (!file_path_join(path, folder, "manifest", ".txt") || file_read_whole(path, &bytes, &size) != FILE_OK) {
        return 0;
    }
    mem_free(bytes);
    return 1;
}

const char *asset_sets_choose(const char *remastered_folder, const char *ds_folder)
{
    int has_remastered;
    int has_ds;

    has_remastered = folder_has_a_set(remastered_folder);
    has_ds = folder_has_a_set(ds_folder);
    game_settings_set_bundled_art(has_remastered || !has_ds, has_ds);
    running_filter = game_settings_visual_filter();
    snprintf(chosen_folder, sizeof(chosen_folder), "%s", running_filter == GAME_FILTER_DS ? ds_folder : remastered_folder);
    run_log_write("asset sets remastered=%d ds=%d running=%s", has_remastered, has_ds, chosen_folder);
    return chosen_folder;
}

int asset_sets_running_filter(void)
{
    return running_filter;
}

int asset_sets_switch_is_due(void)
{
    return game_settings_visual_filter() != running_filter;
}
