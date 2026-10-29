struct Object {
    void *methods;
};

extern char data_020a4670;
void func_0201e534(void *object);
extern char data_02084e10;

struct Object *func_0201ef58(struct Object *object)
{
    func_0201e534(object);
    object->methods = &data_020a4670;
    *(void **)((char *)object + 4) = &data_02084e10;
    return object;
}
