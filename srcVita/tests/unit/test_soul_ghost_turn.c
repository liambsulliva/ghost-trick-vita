#include <string.h>
#include "script_tables.h"
#include "test_support.h"

#define RED_CLASS 0x11110001u
#define ORANGE_CLASS 0x11110002u
#define OLD_LINE_CLASS 0x11110003u

static struct script_tables tables;
static const char soul[] = "owner a\nring b\nclass 11110001 0.85 0.12 0.07 1 0 1\nclass 11110002 0.7 0.38 0.03 1 1 0\nclass 11110003 0.85 0.12 0.07 1 0\n";

static const struct script_soul_style *style_of(unsigned int class_id)
{
    return script_tables_soul_style(&tables, class_id, 0, 0, 0, 0);
}

static void test_a_class_line_says_whether_its_tint_turns_with_the_ghost(void)
{
    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_soul(&tables, soul, sizeof(soul) - 1));
    CHECK(style_of(RED_CLASS) != NULL);
    CHECK(style_of(ORANGE_CLASS) != NULL);
    CHECK_INT(style_of(RED_CLASS)->turns_with_ghost, 1);
    CHECK_INT(style_of(ORANGE_CLASS)->turns_with_ghost, 0);
    CHECK_INT(style_of(ORANGE_CLASS)->is_flat, 1);
}

static void test_a_class_line_without_the_flag_keeps_its_tint(void)
{
    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_soul(&tables, soul, sizeof(soul) - 1));
    CHECK(style_of(OLD_LINE_CLASS) != NULL);
    CHECK_INT(style_of(OLD_LINE_CLASS)->turns_with_ghost, 0);
}

int main(void)
{
    test_a_class_line_says_whether_its_tint_turns_with_the_ghost();
    test_a_class_line_without_the_flag_keeps_its_tint();
    return test_finish("unit/soul_ghost_turn");
}
