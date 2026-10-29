struct Inner {
    int (**methods)(struct Inner *self);
};

struct Outer {
    unsigned char before[4];
    struct Inner *inner;
};

int func_0201688c(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    if (inner != 0) {
        return inner->methods[33](inner);
    }
    return 0;
}
