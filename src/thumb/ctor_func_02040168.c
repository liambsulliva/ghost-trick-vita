struct Object {
    void *methods;
    unsigned char pad0[36];
    int field40;
};

extern char data_020a79d4;
void func_0202f524(void *object);

struct Object *func_02040168(struct Object *object)
{
    func_0202f524(object);
    object->methods = &data_020a79d4;
    object->field40 = 0;
    return object;
}
