#include <string.h>
#include "fakes/fake_platform.h"
#include "system_sounds.h"
#include "test_support.h"

static const char audio_names[] = "music 1 bgm01_new\nsound 1 sys_001\nsound 26 sys_026\nsound 4 gim_004\n";

static void test_the_kinds_are_read_from_the_scripts_audio_names(void)
{
    fake_files_reset();
    fake_files_add("set/scripts/audio_names.txt", (const unsigned char *)audio_names, (unsigned int)strlen(audio_names));
    CHECK_INT(system_sounds_load("set/scripts"), 3);
    CHECK(strcmp(system_sounds_name(1), "sys_001") == 0);
    CHECK(strcmp(system_sounds_name(26), "sys_026") == 0);
    CHECK(system_sounds_name(2) == NULL);
}

static void test_a_missing_file_leaves_no_sounds(void)
{
    fake_files_reset();
    CHECK_INT(system_sounds_load("set/scripts"), 0);
    CHECK(system_sounds_name(1) == NULL);
}

int main(void)
{
    test_the_kinds_are_read_from_the_scripts_audio_names();
    test_a_missing_file_leaves_no_sounds();
    return test_finish("unit/system_sounds");
}
