struct Inner {
    void (**methods)(struct Inner *self, int extra1, int extra2);
};

struct Outer {
    unsigned char before[4];
    struct Inner *inner;
};

void func_0201ea1c(struct Outer *object, int extra1, int extra2)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[6](inner, extra1, extra2);
}
