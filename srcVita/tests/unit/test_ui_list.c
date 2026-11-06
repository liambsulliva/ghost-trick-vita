#include "test_support.h"
#include "ui_list.h"

static void test_list_starts_at_the_top(void)
{
    struct ui_list list;

    ui_list_init(&list, 5, 3);
    CHECK_INT(list.selected, 0);
    CHECK_INT(ui_list_row_for_item(&list, 0), 0);
    CHECK_INT(ui_list_row_for_item(&list, 2), 2);
    CHECK_INT(ui_list_row_for_item(&list, 3), -1);
}

static void test_list_scrolls_to_keep_selection_visible(void)
{
    struct ui_list list;

    ui_list_init(&list, 5, 3);
    ui_list_move_selection(&list, 3);
    CHECK_INT(list.selected, 3);
    CHECK_INT(list.first_visible, 1);
    CHECK_INT(ui_list_row_for_item(&list, 3), 2);
    CHECK_INT(ui_list_row_for_item(&list, 0), -1);
    ui_list_move_selection(&list, -3);
    CHECK_INT(list.selected, 0);
    CHECK_INT(list.first_visible, 0);
}

static void test_list_wraps_around_both_ends(void)
{
    struct ui_list list;

    ui_list_init(&list, 5, 3);
    ui_list_move_selection(&list, -1);
    CHECK_INT(list.selected, 4);
    CHECK_INT(list.first_visible, 2);
    ui_list_move_selection(&list, 1);
    CHECK_INT(list.selected, 0);
    CHECK_INT(list.first_visible, 0);
}

static void test_list_select_clamps_out_of_range_items(void)
{
    struct ui_list list;

    ui_list_init(&list, 5, 3);
    ui_list_select(&list, 99);
    CHECK_INT(list.selected, 4);
    ui_list_select(&list, -99);
    CHECK_INT(list.selected, 0);
}

static void test_empty_list_ignores_movement(void)
{
    struct ui_list list;

    ui_list_init(&list, 0, 3);
    ui_list_move_selection(&list, 1);
    ui_list_select(&list, 1);
    CHECK_INT(list.selected, 0);
}

int main(void)
{
    test_list_starts_at_the_top();
    test_list_scrolls_to_keep_selection_visible();
    test_list_wraps_around_both_ends();
    test_list_select_clamps_out_of_range_items();
    test_empty_list_ignores_movement();
    return test_finish("unit/ui_list");
}
