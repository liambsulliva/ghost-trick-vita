struct WithMethods {
    void *methods;
};

extern char data_020aedb4;
void func_0201edc0(void *object, int a, int b, int c, int d, int e);

struct WithMethods *func_0205e790(struct WithMethods *object, int a, int b, int c, int d, int e)
{
    func_0201edc0(object, a, b, c, d, e);
    object->methods = &data_020aedb4;
    return object;
}
