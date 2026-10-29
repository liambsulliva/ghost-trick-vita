struct Inner {
    void (**methods)(struct Inner *self, int extra1);
};

struct Outer {
    unsigned char before[4];
    struct Inner *inner;
};

void func_02016498(struct Outer *object, int extra1)
{
    struct Inner *inner;
    inner = object->inner;
    if (inner != 0) {
        inner->methods[8](inner, extra1);
    }
}
