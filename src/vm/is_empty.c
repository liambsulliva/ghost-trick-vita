struct Holder {
    int unknown_0;
    void *item;
};

int func_020790c8(void *item);

int func_02027ec8(struct Holder *holder)
{
    if (func_020790c8(holder->item) == 0) {
        return 1;
    }
    return 0;
}
