struct Object {
    void *methods;
};

extern char data_020a6c40;
void func_0202f64c(void *object);
void func_02038ab8(void *object);

struct Object *func_0203a680(struct Object *object)
{
    object->methods = &data_020a6c40;
    func_0202f64c((char *)object + 248);
    func_02038ab8(object);
    return object;
}
