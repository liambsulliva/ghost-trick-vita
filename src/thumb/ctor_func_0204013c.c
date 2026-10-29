struct Object {
    void *methods;
    unsigned char pad0[28];
    int field32;
};

extern char data_020a79a0;
void func_02022d70(void *object);

struct Object *func_0204013c(struct Object *object)
{
    func_02022d70(object);
    object->methods = &data_020a79a0;
    object->field32 = 0;
    return object;
}
