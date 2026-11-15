#ifndef ASSET_SETS_H
#define ASSET_SETS_H

#include "file.h"

#define ASSET_SETS_REMASTERED_FOLDER FILE_DATA_FOLDER "/assets"
#define ASSET_SETS_DS_FOLDER FILE_DATA_FOLDER "/assets_ds"

const char *asset_sets_choose(const char *remastered_folder, const char *ds_folder);
int asset_sets_running_filter(void);
int asset_sets_switch_is_due(void);

#endif
