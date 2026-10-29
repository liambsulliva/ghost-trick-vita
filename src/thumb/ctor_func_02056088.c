struct Object {
    void *methods;
    unsigned char pad0[36];
    int field40;
    int field44;
};

extern char data_020ad5b0;
void func_0202f524(void *object);

struct Object *func_02056088(struct Object *object)
{
    func_0202f524(object);
    object->methods = &data_020ad5b0;
    object->field40 = 0;
    object->field44 = 0;
    return object;
}
