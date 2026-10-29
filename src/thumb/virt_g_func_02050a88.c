struct Inner {
    void (**methods)(struct Inner *self, void *arg);
};

extern struct Inner *data_020ceda8;

void func_02050a88(void *arg)
{
    struct Inner *inner;
    inner = data_020ceda8;
    inner->methods[4](inner, arg);
}
