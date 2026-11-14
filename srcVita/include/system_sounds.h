#ifndef SYSTEM_SOUNDS_H
#define SYSTEM_SOUNDS_H

#define SYSTEM_SOUND_ROW_MAX 640
#define SYSTEM_SOUND_NAME_MAX 32

int system_sounds_load(const char *scripts_folder);
const char *system_sounds_name(int kind);
void system_sounds_play(int kind);

#endif
