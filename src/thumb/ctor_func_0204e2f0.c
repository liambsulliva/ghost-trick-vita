struct Object {
    void *methods;
    unsigned char pad0[64];
    int field68;
    int field72;
    int field76;
    int field80;
    int field84;
    int field88;
    int field92;
    unsigned char pad7[4];
    int field100;
    int field104;
    int field108;
    int field112;
};

extern char data_020ab4b0;
void func_0202ffb4(void *object);

struct Object *func_0204e2f0(struct Object *object)
{
    func_0202ffb4(object);
    object->methods = &data_020ab4b0;
    object->field68 = 0;
    object->field72 = 0;
    object->field76 = 0;
    object->field80 = 0;
    object->field84 = 0;
    object->field88 = 0;
    object->field92 = 0;
    object->field100 = 0;
    object->field104 = 0;
    object->field108 = 0;
    object->field112 = 0;
    return object;
}
