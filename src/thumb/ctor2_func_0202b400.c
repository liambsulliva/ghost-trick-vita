struct Object {
    void *methods;
};

extern char data_020a50b0;
void func_0202b53c(void *object);
void func_02007ab4(void *object);

struct Object *func_0202b400(struct Object *object)
{
    object->methods = &data_020a50b0;
    func_0202b53c(object);
    func_02007ab4(object);
    return object;
}
