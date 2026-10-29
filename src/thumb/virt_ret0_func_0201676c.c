struct Inner {
    int (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[4];
    struct Inner *inner;
};

int func_0201676c(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    if (inner != 0) {
        return inner->methods[16](inner);
    }
    return 0;
}
