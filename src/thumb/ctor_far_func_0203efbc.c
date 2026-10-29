struct Object {
    void *methods;
};

extern char data_020a76ec;
void func_0202fe34(void *object);

struct Object *func_0203efbc(struct Object *object)
{
    func_0202fe34(object);
    object->methods = &data_020a76ec;
    *(int *)((char *)object + 188) = 0;
    *(int *)((char *)object + 192) = 0;
    *(int *)((char *)object + 196) = 0;
    *(int *)((char *)object + 200) = 0;
    return object;
}
