#include <string.h>
#include "script_tables.h"
#include "test_support.h"

#define RED_CLASS 0x11110001u
#define ORANGE_CLASS 0x11110002u
#define OLD_LINE_CLASS 0x11110003u

static struct script_tables tables;
static const char soul[] = "owner a\nring b\nclass 11110001 0.85 0.12 0.07 1 0 1\nclass 11110002 0.7 0.38 0.03 1 1 0\nclass 11110003 0.85 0.12 0.07 1 0\nobject phone_view 0.85 0.12 0.07 1 1 1\n";

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

static void test_the_phone_view_row_is_a_flat_silhouette_that_turns_with_the_ghost(void)
{
    const struct script_soul_style *style;

    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_soul(&tables, soul, sizeof(soul) - 1));
    style = script_tables_phone_view_style(&tables);
    CHECK(style != NULL);
    CHECK_INT(style->is_flat, 1);
    CHECK_INT(style->turns_with_ghost, 1);
    CHECK(style_of(RED_CLASS) != style);
}

static void test_no_phone_view_row_means_no_phone_view_style(void)
{
    static const char without[] = "owner a\nring b\nclass 11110001 0.85 0.12 0.07 1 0 1\n";

    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_soul(&tables, without, sizeof(without) - 1));
    CHECK(script_tables_phone_view_style(&tables) == NULL);
}

int main(void)
{
    test_a_class_line_says_whether_its_tint_turns_with_the_ghost();
    test_a_class_line_without_the_flag_keeps_its_tint();
    test_the_phone_view_row_is_a_flat_silhouette_that_turns_with_the_ghost();
    test_no_phone_view_row_means_no_phone_view_style();
    return test_finish("unit/soul_ghost_turn");
}
