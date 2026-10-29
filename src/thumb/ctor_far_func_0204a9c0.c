struct Object {
    void *methods;
};

extern char data_020a92ec;
void func_02049bf0(void *object);

struct Object *func_0204a9c0(struct Object *object)
{
    func_02049bf0(object);
    object->methods = &data_020a92ec;
    *((char *)object + 184) = 3;
    return object;
}
