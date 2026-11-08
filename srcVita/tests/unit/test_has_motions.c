#include <string.h>
#include "script_tables.h"
#include "test_support.h"

static void test_only_scripts_with_motion_rows_have_motions(void)
{
    static struct script_tables tables;
    static const char motions[] = "motion 5d78 st06_pome_001 3\n";

    script_tables_clear(&tables);
    CHECK(script_tables_read_motions(&tables, "st06-st06_game001_Expand", motions, (unsigned int)strlen(motions)));
    CHECK(script_tables_has_motions(&tables, "st06-st06_game001_Expand"));
    CHECK(!script_tables_has_motions(&tables, "st06-st06_game006_Expand"));
    CHECK(!script_tables_has_motions(&tables, "chapter"));
}

int main(void)
{
    test_only_scripts_with_motion_rows_have_motions();
    return test_finish("unit/has_motions");
}
