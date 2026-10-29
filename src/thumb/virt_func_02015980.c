struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[4];
    struct Inner *inner;
};

void func_02015980(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[7](inner);
}
