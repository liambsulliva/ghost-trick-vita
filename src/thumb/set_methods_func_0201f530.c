struct Object {
    void *methods;
};

extern char data_020a2b30;

struct Object *func_0201f530(struct Object *object)
{
    object->methods = &data_020a2b30;
    return object;
}
