struct Object {
    void *methods;
};

extern char data_020a690c;
void func_0202f64c(void *object);
void func_02037124(void *object);
void func_0202f644(void *object);

struct Object *func_0203900c(struct Object *object)
{
    object->methods = &data_020a690c;
    func_0202f64c((char *)object + 204);
    func_02037124(object);
    func_0202f644(object);
    return object;
}
