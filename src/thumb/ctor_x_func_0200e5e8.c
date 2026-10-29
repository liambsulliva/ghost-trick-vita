struct Object {
    void *methods;
};

extern char data_020a2f78;
void func_02007aa8(void *object);
void func_02012128(void *object);

struct Object *func_0200e5e8(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a2f78;
    *((char *)object + 4) = 0;
    *((char *)object + 5) = 0;
    func_02012128(object);
    return object;
}
