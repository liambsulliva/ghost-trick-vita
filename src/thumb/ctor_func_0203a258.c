struct Object {
    void *methods;
    unsigned char pad0[6];
    unsigned short field10;
    unsigned short field12;
};

extern char data_020a67ec;
void func_02038ae8(void *object);

struct Object *func_0203a258(struct Object *object)
{
    func_02038ae8(object);
    object->methods = &data_020a67ec;
    object->field10 = 0;
    object->field12 = 0;
    return object;
}
