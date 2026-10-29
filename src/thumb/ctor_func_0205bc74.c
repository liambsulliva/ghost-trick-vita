struct Object {
    void *methods;
    unsigned char pad0[36];
    int field40;
    unsigned char pad1[4];
    int field48;
};

extern char data_020aecf8;
void func_0202f524(void *object);

struct Object *func_0205bc74(struct Object *object)
{
    func_0202f524(object);
    object->methods = &data_020aecf8;
    object->field40 = 0;
    object->field48 = 0;
    return object;
}
