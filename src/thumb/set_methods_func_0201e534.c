struct Object {
    void *methods;
};

extern char data_020a4618;

struct Object *func_0201e534(struct Object *object)
{
    object->methods = &data_020a4618;
    return object;
}
