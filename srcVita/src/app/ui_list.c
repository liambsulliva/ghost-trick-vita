#include "ui_list.h"

static void scroll_to_show_selected(struct ui_list *list)
{
    if (list->selected < list->first_visible) {
        list->first_visible = list->selected;
    }
    if (list->selected >= list->first_visible + list->visible_rows) {
        list->first_visible = list->selected - list->visible_rows + 1;
    }
}

void ui_list_init(struct ui_list *list, int item_count, int visible_rows)
{
    list->item_count = item_count;
    list->visible_rows = visible_rows;
    list->selected = 0;
    list->first_visible = 0;
}

void ui_list_select(struct ui_list *list, int item)
{
    if (list->item_count == 0) {
        return;
    }
    if (item < 0) {
        item = 0;
    }
    if (item >= list->item_count) {
        item = list->item_count - 1;
    }
    list->selected = item;
    scroll_to_show_selected(list);
}

void ui_list_move_selection(struct ui_list *list, int rows)
{
    int moved;

    if (list->item_count == 0) {
        return;
    }
    moved = list->selected + rows;
    while (moved < 0) {
        moved = moved + list->item_count;
    }
    while (moved >= list->item_count) {
        moved = moved - list->item_count;
    }
    list->selected = moved;
    scroll_to_show_selected(list);
}

int ui_list_row_for_item(const struct ui_list *list, int item)
{
    if (item < list->first_visible || item >= list->first_visible + list->visible_rows) {
        return -1;
    }
    return item - list->first_visible;
}
