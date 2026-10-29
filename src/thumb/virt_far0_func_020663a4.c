struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[200];
    struct Inner *inner;
};

void func_020663a4(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[32](inner);
}
