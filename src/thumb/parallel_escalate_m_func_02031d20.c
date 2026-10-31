struct Object {
    void *methods;
    char pad[0x1c];
    int field20;
    int field24;
    int field28;
    int field2c;
    unsigned char flag30;
    unsigned char flag31;
    unsigned char flag32;
    unsigned char flag33;
    unsigned char flag34;
    unsigned char flag35;
};

extern char data_020a5680;
void func_02022d70(void *object);

struct Object *func_02031d20(struct Object *object)
{
    func_02022d70(object);
    object->methods = &data_020a5680;
    object->field20 = 0;
    object->flag30 = 0;
    object->flag31 = 0;
    object->flag32 = 0;
    object->flag33 = 0;
    object->flag34 = 0;
    object->flag35 = 0;
    object->field24 = 0;
    object->field28 = 0;
    object->field2c = 0;
    return object;
}
