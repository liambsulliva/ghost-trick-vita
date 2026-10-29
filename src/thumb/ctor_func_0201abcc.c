struct Object {
    void *methods;
    unsigned char pad0[100];
    int field104;
    int field108;
    int field112;
    int field116;
};

extern char data_020a3aa8;
void func_02016c4c(void *object);

struct Object *func_0201abcc(struct Object *object)
{
    func_02016c4c(object);
    object->methods = &data_020a3aa8;
    object->field104 = 0;
    object->field108 = 0;
    object->field112 = 0;
    object->field116 = 0;
    return object;
}
