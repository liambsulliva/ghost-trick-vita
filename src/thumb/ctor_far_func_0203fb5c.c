struct Object {
    void *methods;
};

extern char data_020a7878;
void func_0203fb78(void *object);

struct Object *func_0203fb5c(struct Object *object)
{
    func_0203fb78(object);
    object->methods = &data_020a7878;
    *((char *)object + 204) = 0;
    return object;
}
