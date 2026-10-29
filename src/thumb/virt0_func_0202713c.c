struct Object {
    void (**methods)(struct Object *self, int extra);
};

void func_0202713c(struct Object *object)
{
    object->methods[8](object, 0);
}
