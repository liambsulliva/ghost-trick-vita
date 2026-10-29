struct Object {
    void *methods;
};

extern char data_020a690c;
void func_0202f64c(void *object);
void func_02037124(void *object);

struct Object *func_02038fec(struct Object *object)
{
    object->methods = &data_020a690c;
    func_0202f64c((char *)object + 204);
    func_02037124(object);
    return object;
}
