struct Object {
    void *methods;
};

extern char data_020a4c9c;

struct Object *func_0202694c(struct Object *object)
{
    object->methods = &data_020a4c9c;
    return object;
}
