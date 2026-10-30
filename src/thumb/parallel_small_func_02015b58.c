struct Inner {
    int (**methods)(struct Inner *self, int extra);
};

struct Outer {
    struct Inner *inner;
};

int func_02015b58(struct Outer *object, int id)
{
    struct Inner *inner;

    inner = object->inner;
    if (inner->methods[22](inner, id) != 0) {
        return 1;
    }
    return 0;
}
