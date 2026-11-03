typedef struct Item0201fc30 {
    char pad[6];
    unsigned char active : 1;
} Item0201fc30;

typedef struct List0201fc30 {
    int pad0;
    int pad4;
    unsigned int count;
    Item0201fc30 **items;
} List0201fc30;

int func_0201fc30(List0201fc30 *list)
{
    unsigned int i;

    for (i = 0; i < list->count; i++) {
        if (list->items[i] != 0 && list->items[i]->active) {
            return i;
        }
    }
    return -1;
}
