struct Inner {
    void (**methods)(struct Inner *self);
};

void func_02016654(struct Inner *object)
{
    object->methods[6](object);
}
