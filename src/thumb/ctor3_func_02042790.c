struct Object {
    void *methods;
};

extern char data_020a7e54;
void func_02042b20(void *object);
void func_02007ab4(void *object);
void func_02031270(void *object);

struct Object *func_02042790(struct Object *object)
{
    object->methods = &data_020a7e54;
    func_02042b20(object);
    func_02007ab4(object);
    func_02031270(object);
    return object;
}
