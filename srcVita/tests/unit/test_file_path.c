#include "file.h"
#include "file_path.h"
#include "test_support.h"

static void test_paths_join_and_refuse_to_overflow(void)
{
    char path[FILE_PATH_MAX];
    char long_name[FILE_PATH_MAX];

    CHECK(file_path_join(path, "a/b", "c", ".txt"));
    CHECK_TEXT(path, "a/b/c.txt");
    memset(long_name, 'x', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    CHECK(!file_path_join(path, "a", long_name, ""));
    CHECK_TEXT(path, "");
}

int main(void)
{
    test_paths_join_and_refuse_to_overflow();
    return test_finish("unit/file_path");
}
