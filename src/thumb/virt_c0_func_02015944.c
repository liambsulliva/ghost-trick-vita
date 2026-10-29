struct Inner {
    void (**methods)(struct Inner *self, int extra);
};

struct Outer {
    struct Inner *inner;
};

void func_02015944(struct Outer *object, int extra)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[10](inner, 1);
}
