struct Object {
    void *methods;
};

extern char data_020a47a4;

struct Object *func_0201f32c(struct Object *object)
{
    object->methods = &data_020a47a4;
    return object;
}
