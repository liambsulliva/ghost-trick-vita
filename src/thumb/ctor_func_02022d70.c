struct Object {
    void *methods;
    int field4;
    int field8;
    unsigned char field12;
    unsigned char field13;
    unsigned char field14;
    unsigned char field15;
    unsigned char field16;
    unsigned char field17;
    unsigned char pad8[2];
    int field20;
    int field24;
};

extern char data_020a49a0;
void func_02007aa8(void *object);

struct Object *func_02022d70(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a49a0;
    object->field4 = 0;
    object->field8 = 0;
    object->field12 = 0;
    object->field13 = 0;
    object->field14 = 0;
    object->field15 = 0;
    object->field16 = 0;
    object->field17 = 0;
    object->field20 = 0;
    object->field24 = 0;
    return object;
}
