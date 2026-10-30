struct Obj {
    int (**methods)(struct Obj *self, int value);
};

int func_0202a65c(char *object, int value, int index)
{
    struct Obj *item;

    if (((unsigned char *)object)[0x24] != 0) {
        index = 0;
    }
    item = *(struct Obj **)(object + index * 4 + 8);
    return item->methods[4](item, value);
}
