struct Inner {
    void (**methods)(struct Inner *self);
};

void func_0201d604(struct Inner *object)
{
    object->methods[7](object);
}
