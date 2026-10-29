struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[4];
    struct Inner *inner;
};

void func_020589ec(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[14](inner);
}
