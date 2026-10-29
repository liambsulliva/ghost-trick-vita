struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    struct Inner *inner;
};

void func_02015924(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[16](inner);
}
