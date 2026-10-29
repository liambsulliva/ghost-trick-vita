struct Inner {
    void (**methods)(struct Inner *self, int extra1, int extra2);
};

struct Outer {
    unsigned char before[16];
    struct Inner *inner;
};

void func_0201e568(struct Outer *object, int extra1, int extra2)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[7](inner, extra1, extra2);
}
