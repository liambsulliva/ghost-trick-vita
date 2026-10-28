struct SmallObject {
    void *methods;
};

extern char data_020a2b74;
void func_02007aa8(void *object);

struct SmallObject *func_02008b20(struct SmallObject *object)
{
    func_02007aa8(object);
    object->methods = &data_020a2b74;
    return object;
}
