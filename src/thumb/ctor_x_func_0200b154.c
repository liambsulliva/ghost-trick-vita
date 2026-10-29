struct Object {
    void *methods;
};

extern char data_020a2c9c;
void func_02007aa8(void *object);

struct Object *func_0200b154(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a2c9c;
    *((char *)object + 19) = 0;
    *(int *)((char *)object + 60) = 0;
    *(int *)((char *)object + 4) = 0;
    *(int *)((char *)object + 8) = 0;
    *(int *)((char *)object + 12) = 0;
    *(int *)((char *)object + 28) = 0;
    *(int *)((char *)object + 32) = 4096;
    *(int *)((char *)object + 36) = 4096;
    return object;
}
