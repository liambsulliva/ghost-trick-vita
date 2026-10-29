struct Object {
    void *methods;
    unsigned char pad0[32];
    int field36;
    int field40;
};

extern char data_020a7c4c;
void func_02022d70(void *object);

struct Object *func_02041b28(struct Object *object)
{
    func_02022d70(object);
    object->methods = &data_020a7c4c;
    object->field36 = 0;
    object->field40 = 0;
    return object;
}
