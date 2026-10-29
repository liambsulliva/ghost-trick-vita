struct Object {
    void *methods;
    unsigned char pad0[36];
    int field40;
};

extern char data_020a7730;
void func_02022d70(void *object);

struct Object *func_0203f9d8(struct Object *object)
{
    func_02022d70(object);
    object->methods = &data_020a7730;
    object->field40 = 0;
    return object;
}
