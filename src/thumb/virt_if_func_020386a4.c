struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[8];
    struct Inner *inner;
};

void func_020386a4(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    if (inner != 0) {
        inner->methods[10](inner);
    }
}
