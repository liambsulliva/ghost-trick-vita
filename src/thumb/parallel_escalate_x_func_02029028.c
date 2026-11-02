struct Item {
    unsigned char pad0[0x3a];
    unsigned char active;
};

void func_02029028(struct Item *item)
{
    if (item->active != 0) {
        return;
    }
}
