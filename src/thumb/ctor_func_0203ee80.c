struct Object {
    void *methods;
    unsigned char pad0[28];
    int field32;
};

extern char data_020a75fc;
void func_0203ec80(void *object);

struct Object *func_0203ee80(struct Object *object)
{
    func_0203ec80(object);
    object->methods = &data_020a75fc;
    object->field32 = 1;
    return object;
}
