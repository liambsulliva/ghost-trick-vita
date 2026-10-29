extern int func_0202fc60(int object);

struct Iter {
    int table;
    int cursor;
    unsigned int index;
    unsigned char flag;
};

int func_0202fc08(struct Iter *object)
{
    int value;
    unsigned int index;
    unsigned int *slot;
    int *cursor;

    index = object->index;
    cursor = &object->cursor;
    if (index >= 16) {
        *cursor = 0;
    }
    else {
        if (*cursor == 0) {
            *cursor = *(int *)(object->table + index * 16 + 0x28);
        }
        else {
            *cursor = func_0202fc60(*cursor);
        }
        if (*cursor == 0) {
            if (object->flag != 0) {
                object->index = 16;
                *cursor = 0;
            }
            else {
                slot = &object->index;
                do {
                    index = *slot + 1;
                    *slot = index;
                    value = *(int *)(object->table + index * 16 + 0x28);
                    *cursor = value;
                    if (index >= 15) {
                        break;
                    }
                } while (value == 0);
            }
        }
    }
    return *cursor;
}
