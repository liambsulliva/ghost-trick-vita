struct Object {
    void *methods;
};

extern char data_020a6f54;
void func_0203b4f4(void *object);

struct Object *func_0203b5dc(struct Object *object)
{
    func_0203b4f4(object);
    object->methods = &data_020a6f54;
    *((char *)object + 46) = 1;
    return object;
}
