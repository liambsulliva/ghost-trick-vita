struct Wide38 {
    void *methods;
    unsigned char before_28[0x24];
    int field_28;
    int field_2c;
};

extern char data_020a2c20;
void func_0202f524(void *object);

struct Wide38 *func_02009814(struct Wide38 *object)
{
    func_0202f524(object);
    object->methods = &data_020a2c20;
    object->field_28 = 0;
    object->field_2c = 1;
    return object;
}
