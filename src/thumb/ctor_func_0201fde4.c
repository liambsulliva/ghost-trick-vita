struct Object {
    void *methods;
    int field4;
    int field8;
    int field12;
};

extern char data_020a487c;
void func_0201f5e4(void *object);

struct Object *func_0201fde4(struct Object *object)
{
    func_0201f5e4(object);
    object->methods = &data_020a487c;
    object->field4 = 0;
    object->field8 = 0;
    object->field12 = 0;
    return object;
}
