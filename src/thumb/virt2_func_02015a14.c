struct Inner {
    void (**methods)(struct Inner *self, int extra1, int extra2);
};

struct Outer {
    struct Inner *inner;
};

void func_02015a14(struct Outer *object, int extra1, int extra2)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[13](inner, extra1, extra2);
}
