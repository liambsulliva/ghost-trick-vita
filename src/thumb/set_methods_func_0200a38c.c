struct Object {
    void *methods;
};

extern char data_020a2bd4;

struct Object *func_0200a38c(struct Object *object)
{
    object->methods = &data_020a2bd4;
    return object;
}
