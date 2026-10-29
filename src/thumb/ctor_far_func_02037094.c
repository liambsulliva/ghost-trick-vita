struct Object {
    void *methods;
};

extern char data_020a695c;
void func_0202fe34(void *object);

struct Object *func_02037094(struct Object *object)
{
    func_0202fe34(object);
    object->methods = &data_020a695c;
    *(int *)((char *)object + 188) = 0;
    *((char *)object + 192) = 0;
    *((char *)object + 193) = 0;
    *(int *)((char *)object + 196) = 0;
    *(int *)((char *)object + 200) = 0;
    return object;
}
