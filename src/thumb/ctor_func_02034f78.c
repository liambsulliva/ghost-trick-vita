struct Object {
    void *methods;
    unsigned char pad0[36];
    int field40;
    int field44;
    int field48;
};

extern char data_020a5d5c;
void func_0202f524(void *object);

struct Object *func_02034f78(struct Object *object)
{
    func_0202f524(object);
    object->methods = &data_020a5d5c;
    object->field40 = 0;
    object->field44 = 0;
    object->field48 = 0;
    return object;
}
