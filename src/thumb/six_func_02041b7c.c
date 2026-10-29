struct WithMethods {
    void *methods;
};

extern char data_020a7c40;
void func_0201edc0(void *object, int a, int b, int c, int d, int e);

struct WithMethods *func_02041b7c(struct WithMethods *object, int a, int b, int c, int d, int e)
{
    func_0201edc0(object, a, b, c, d, e);
    object->methods = &data_020a7c40;
    return object;
}
