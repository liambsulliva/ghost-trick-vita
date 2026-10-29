struct Object {
    void *methods;
};

extern char data_020a71f4;
void func_0203c414(void *object);

struct Object *func_0203d528(struct Object *object)
{
    func_0203c414(object);
    object->methods = &data_020a71f4;
    *((char *)object + 96) = 0;
    *((char *)object + 97) = 0;
    return object;
}
