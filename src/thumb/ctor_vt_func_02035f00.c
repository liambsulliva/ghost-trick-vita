struct Object {
    void *methods;
};

extern char data_020a6720;
void func_0202f64c(void *object);
void func_02022d64(void *object);

struct Object *func_02035f00(struct Object *object)
{
    object->methods = &data_020a6720;
    func_0202f64c((char *)object + 40);
    func_0202f64c((char *)object + 44);
    func_02022d64(object);
    return object;
}
