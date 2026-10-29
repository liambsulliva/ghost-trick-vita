struct Object {
    void *methods;
};

extern char data_020a2cf0;

struct Object *func_0200c524(struct Object *object)
{
    object->methods = &data_020a2cf0;
    return object;
}
