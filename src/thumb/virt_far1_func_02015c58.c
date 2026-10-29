struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    struct Inner *inner;
};

void func_02015c58(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[36](inner);
}
