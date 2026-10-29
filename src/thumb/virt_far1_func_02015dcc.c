struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    struct Inner *inner;
};

void func_02015dcc(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[54](inner);
}
