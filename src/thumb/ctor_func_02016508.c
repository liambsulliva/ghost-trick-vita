struct Object {
    void *methods;
    unsigned char pad0[8];
    int field12;
    int field16;
};

extern char data_020a3784;
void func_020164f0(void *object);

struct Object *func_02016508(struct Object *object)
{
    func_020164f0(object);
    object->methods = &data_020a3784;
    object->field12 = 0;
    object->field16 = 0;
    return object;
}
