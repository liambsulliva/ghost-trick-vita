struct Inner {
    void (**methods)(struct Inner *self, int extra);
};

struct Outer {
    unsigned char before[200];
    struct Inner *inner;
};

void func_020662e4(struct Outer *object, int extra)
{
    struct Inner *inner;
    inner = object->inner;
    inner->methods[46](inner, extra);
}
