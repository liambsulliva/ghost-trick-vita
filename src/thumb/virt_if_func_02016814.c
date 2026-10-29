struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[12];
    struct Inner *inner;
};

void func_02016814(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    if (inner != 0) {
        inner->methods[28](inner);
    }
}
