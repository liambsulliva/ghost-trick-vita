struct Object {
    void *methods;
};

extern char data_020a9934;
void func_02047120(void *object);

struct Object *func_020472b4(struct Object *object)
{
    func_02047120(object);
    object->methods = &data_020a9934;
    *(int *)((char *)object + 192) = 0;
    return object;
}
