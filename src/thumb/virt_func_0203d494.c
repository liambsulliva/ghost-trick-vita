struct Inner {
    void (**methods)(struct Inner *self);
};

struct Outer {
    struct Inner *inner;
};

void func_0203d494(struct Outer *object)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[26](inner);
}
