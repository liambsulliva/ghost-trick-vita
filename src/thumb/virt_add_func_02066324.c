struct Inner {
    void (**methods)(struct Inner *self, int extra1, int extra2);
};

struct Outer {
    unsigned char before[200];
    struct Inner *inner;
};

void func_02066324(struct Outer *object, int extra1, int extra2)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[24](inner, extra1, extra2);
}
