struct Object {
    void *methods;
};

extern char data_020ad620;
void func_02056174(void *object);

struct Object *func_020563cc(struct Object *object)
{
    func_02056174(object);
    object->methods = &data_020ad620;
    *((char *)object + 64) = 0;
    *(int *)((char *)object + 60) = 0;
    return object;
}
