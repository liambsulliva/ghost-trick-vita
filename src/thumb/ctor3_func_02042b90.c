struct Object {
    void *methods;
};

extern char data_020a7e70;
void func_02042d20(void *object);
void func_02023d00(void *object);
void func_02023d80(void *object);

struct Object *func_02042b90(struct Object *object)
{
    object->methods = &data_020a7e70;
    func_02042d20(object);
    func_02023d00(object);
    func_02023d80(object);
    return object;
}
