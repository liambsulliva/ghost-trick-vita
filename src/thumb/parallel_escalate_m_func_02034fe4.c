struct Object02034fe4 {
    void *methods;
    int field4;
    int field8;
    unsigned char fieldc[6];
    unsigned char pad12[2];
    int field14;
    int field18;
    int field1c;
    int field20;
    int field24;
    int field28;
    unsigned char field2c;
    unsigned char pad2d;
    unsigned short field2e;
    unsigned short field30;
    unsigned short pad32;
    int field34;
    int field38;
    unsigned char field3c;
    unsigned char field3d;
    unsigned char field3e;
};

extern char data_020a5d28;
void func_02022d70(struct Object02034fe4 *object);

struct Object02034fe4 *func_02034fe4(struct Object02034fe4 *object)
{
    func_02022d70(object);
    object->methods = &data_020a5d28;
    object->field20 = 0;
    object->field24 = 0;
    object->field28 = -1;
    object->field2c = 0;
    object->field3c = 0;
    object->field3d = 0xc0;
    object->field3e = 0;
    object->field2e = 0;
    object->field30 = 0;
    object->field34 = 0;
    object->field38 = 0;
    return object;
}
