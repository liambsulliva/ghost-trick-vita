struct Inner {
    void (**methods)(struct Inner *self, void *arg);
};

extern struct Inner *data_020bbc34;

void func_0201f6e8(void *arg)
{
    struct Inner *inner;
    inner = data_020bbc34;
    inner->methods[4](inner, arg);
}
