#ifndef UI_LIST_H
#define UI_LIST_H

struct ui_list {
    int item_count;
    int visible_rows;
    int selected;
    int first_visible;
};

void ui_list_init(struct ui_list *list, int item_count, int visible_rows);
void ui_list_move_selection(struct ui_list *list, int rows);
int ui_list_row_for_item(const struct ui_list *list, int item);
void ui_list_select(struct ui_list *list, int item);

#endif
