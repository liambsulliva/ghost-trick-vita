struct Object {
    void *methods;
};

extern char data_020a6720;
void func_0202f64c(void *object);
void func_02022d64(void *object);
void func_02031270(void *object);

struct Object *func_020363e0(struct Object *object)
{
    object->methods = &data_020a6720;
    func_0202f64c((char *)object + 40);
    func_0202f64c((char *)object + 44);
    func_02022d64(object);
    func_02031270(object);
    return object;
}
