struct Object {
    void *methods;
};

extern char data_020abbd8;
void func_0202fe34(void *object);

struct Object *func_020505d8(struct Object *object)
{
    func_0202fe34(object);
    object->methods = &data_020abbd8;
    *(int *)((char *)object + 188) = 0;
    *((char *)object + 192) = 0;
    return object;
}
