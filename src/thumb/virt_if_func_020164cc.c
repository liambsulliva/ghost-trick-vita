struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[4];
    struct Inner *inner;
};

void func_020164cc(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    if (inner != 0) {
        inner->methods[18](inner);
    }
}
