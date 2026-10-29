struct Inner {
    void (**methods)(struct Inner *self);
};

void func_020471a4(struct Inner *object)
{
    object->methods[5](object);
}
