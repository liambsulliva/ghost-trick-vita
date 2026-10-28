struct Wide40 {
    void *methods;
    unsigned char before_28[0x24];
    int field_28;
    unsigned char before_3c[0x10];
    int field_3c;
};

extern char data_020a2be8;
void func_0202f524(void *object);

struct Wide40 *func_020097e4(struct Wide40 *object)
{
    func_0202f524(object);
    object->methods = &data_020a2be8;
    object->field_28 = 0;
    object->field_3c = 0;
    return object;
}
