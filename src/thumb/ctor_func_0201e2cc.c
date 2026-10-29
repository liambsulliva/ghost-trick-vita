struct Object {
    void *methods;
    int field4;
    int field8;
    int field12;
    unsigned char pad3[8];
    unsigned char field24;
    unsigned char field25;
};

extern char data_020a456c;
void func_0200c524(void *object);

struct Object *func_0201e2cc(struct Object *object)
{
    func_0200c524(object);
    object->methods = &data_020a456c;
    object->field4 = 0;
    object->field8 = 0;
    object->field12 = 0;
    object->field24 = 0;
    object->field25 = 0;
    return object;
}
