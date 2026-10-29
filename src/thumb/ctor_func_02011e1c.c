struct Object {
    void *methods;
    int field4;
    int field8;
    unsigned char field12;
};

extern char data_020a3004;
void func_02007aa8(void *object);

struct Object *func_02011e1c(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a3004;
    object->field4 = 0;
    object->field8 = 0;
    object->field12 = 1;
    return object;
}
