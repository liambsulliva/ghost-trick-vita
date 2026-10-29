struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    struct Inner *inner;
};

void func_0201eb24(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[11](inner);
}
