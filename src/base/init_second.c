struct SmallObject {
    void *methods;
};

extern char data_020a2b90;
void func_02007aa8(void *object);

struct SmallObject *func_02008b48(struct SmallObject *object)
{
    func_02007aa8(object);
    object->methods = &data_020a2b90;
    return object;
}
