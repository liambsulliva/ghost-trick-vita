#ifndef ASSET_LANGUAGE_H
#define ASSET_LANGUAGE_H

#define ASSET_LANGUAGE_COUNT 9
#define ASSET_LANGUAGE_ENGLISH 0
#define ASSET_LANGUAGE_FOLDER "lang"
#define ASSET_LANGUAGE_NAMES_FILE "names.txt"
#define ASSET_LANGUAGE_PATH_MAX 128
#define ASSET_LANGUAGE_OVERRIDE_MAX 4096

void asset_language_init(const char *assets_folder);
int asset_language_use(int language);
int asset_language_now(void);
const char *asset_language_code(int language);
int asset_language_overrides(const char *relative_path);
int asset_language_join(char *path, const char *folder, const char *relative_path);
int asset_language_join_assets(char *path, const char *relative_path);
int asset_language_overrides_texture(const char *texture_name);
int asset_language_texture_folder(char *folder);
int asset_language_from_system(int system_language);

#endif
