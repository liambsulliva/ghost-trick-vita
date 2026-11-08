#include <stdio.h>
#include <string.h>
#include "script_tables.h"
#include "test_support.h"

#define FILES 40
#define ROWS_PER_FILE 300

static void test_every_motion_row_is_found_and_the_first_of_two_wins(void)
{
    static struct script_tables tables;
    static char text[ROWS_PER_FILE * 48];
    char file_name[32];
    const struct script_motion_row *row;
    unsigned int used;
    int file;
    int index;

    script_tables_clear(&tables);
    for (file = 0; file < FILES; file++) {
        used = 0;
        for (index = 0; index < ROWS_PER_FILE; index++) {
            used = used + (unsigned int)snprintf(&text[used], sizeof(text) - used, "motion %x t_%d_%d 1\n", index * 16, file, index);
        }
        snprintf(file_name, sizeof(file_name), "script_%02d", file);
        CHECK(script_tables_read_motions(&tables, file_name, text, used));
    }
    CHECK(script_tables_read_motions(&tables, "script_07", "motion 20 t_second 1\n", 21));
    row = script_tables_find_motion(&tables, "script_07", 0x20);
    CHECK(row != NULL && strcmp(row->animation_name, "t_7_2") == 0);
    row = script_tables_find_motion(&tables, "script_39", 299 * 16);
    CHECK(row != NULL && strcmp(row->animation_name, "t_39_299") == 0);
    CHECK(script_tables_find_motion(&tables, "script_39", 300 * 16) == NULL);
    CHECK(script_tables_find_motion(&tables, "script_40", 0) == NULL);
}

int main(void)
{
    test_every_motion_row_is_found_and_the_first_of_two_wins();
    return test_finish("unit/motion_index");
}
