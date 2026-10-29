struct Object {
    void *methods;
    int field4;
    int field8;
    int field12;
    unsigned char field16;
    unsigned char field17;
    unsigned char pad5[2];
    int field20;
    int field24;
    int field28;
};

extern char data_020a4cb4;
void func_02007aa8(void *object);

struct Object *func_02050a9c(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a4cb4;
    object->field4 = 0;
    object->field8 = 0;
    object->field12 = 0;
    object->field16 = 0;
    object->field17 = 0;
    object->field20 = 0;
    object->field24 = 0;
    object->field28 = 0;
    return object;
}
