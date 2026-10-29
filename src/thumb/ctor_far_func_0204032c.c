struct Object {
    void *methods;
};

extern char data_020a7b28;
void func_020402e8(void *object);

struct Object *func_0204032c(struct Object *object)
{
    func_020402e8(object);
    object->methods = &data_020a7b28;
    *(int *)((char *)object + 204) = 0;
    *(int *)((char *)object + 208) = 0;
    *(int *)((char *)object + 212) = 0;
    return object;
}
