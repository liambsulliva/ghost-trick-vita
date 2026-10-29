struct Object {
    void *methods;
};

extern char data_020ad534;
void func_0202fe34(void *object);

struct Object *func_020557d4(struct Object *object)
{
    func_0202fe34(object);
    object->methods = &data_020ad534;
    *((char *)object + 188) = 0;
    return object;
}
