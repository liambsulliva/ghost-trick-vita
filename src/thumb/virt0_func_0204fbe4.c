struct Object {
    void (**methods)(struct Object *self, int extra);
};

void func_0204fbe4(struct Object *object)
{
    object->methods[8](object, 0);
}
