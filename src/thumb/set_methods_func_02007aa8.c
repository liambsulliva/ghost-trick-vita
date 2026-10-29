struct Object {
    void *methods;
};

extern char data_020a2b30;

struct Object *func_02007aa8(struct Object *object)
{
    object->methods = &data_020a2b30;
    return object;
}
