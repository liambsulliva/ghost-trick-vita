struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[200];
    struct Inner *inner;
};

void func_020663c4(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[34](inner);
}
