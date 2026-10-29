struct Object {
    void *methods;
};

extern char data_020a7ba0;
void func_0203fb00(void *object);

struct Object *func_02040280(struct Object *object)
{
    func_0203fb00(object);
    object->methods = &data_020a7ba0;
    *(int *)((char *)object + 188) = 0;
    *((char *)object + 192) = 0;
    *(int *)((char *)object + 196) = 0;
    *(int *)((char *)object + 200) = 0;
    return object;
}
