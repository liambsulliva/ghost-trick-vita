struct Inner {
    void (**methods)(struct Inner *self, void *arg);
};

extern struct Inner *data_020b8774;

void func_0200d8f8(void *arg)
{
    struct Inner *inner;
    inner = data_020b8774;
    inner->methods[4](inner, arg);
}
