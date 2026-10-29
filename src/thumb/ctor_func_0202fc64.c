struct Object {
    void *methods;
    int field4;
    int field8;
    int field12;
};

extern char data_020a5424;
void func_02007aa8(void *object);

struct Object *func_0202fc64(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a5424;
    object->field4 = 0;
    object->field8 = 0;
    object->field12 = 0;
    return object;
}
