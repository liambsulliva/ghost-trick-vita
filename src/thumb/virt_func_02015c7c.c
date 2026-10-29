struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    struct Inner *inner;
};

void func_02015c7c(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[30](inner);
}
