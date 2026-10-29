struct Object {
    void *methods;
    unsigned char pad0[28];
    int field32;
};

extern char data_020a583c;
void func_02033ddc(void *object);

struct Object *func_02034088(struct Object *object)
{
    func_02033ddc(object);
    object->methods = &data_020a583c;
    object->field32 = 1;
    return object;
}
