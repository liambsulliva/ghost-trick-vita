struct Object {
    void *methods;
};

extern char data_020a9828;
void func_02047120(void *object);

struct Object *func_02049028(struct Object *object)
{
    func_02047120(object);
    object->methods = &data_020a9828;
    *(int *)((char *)object + 184) = 0;
    *(int *)((char *)object + 188) = 0;
    return object;
}
