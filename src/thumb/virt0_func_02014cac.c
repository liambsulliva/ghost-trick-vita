struct Object {
    void (**methods)(struct Object *self, int extra);
};

void func_02014cac(struct Object *object)
{
    object->methods[8](object, 0);
}
