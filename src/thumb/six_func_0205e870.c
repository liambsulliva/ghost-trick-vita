struct WithMethods {
    void *methods;
};

extern char data_020aede4;
void func_0201edc0(void *object, int a, int b, int c, int d, int e);

struct WithMethods *func_0205e870(struct WithMethods *object, int a, int b, int c, int d, int e)
{
    func_0201edc0(object, a, b, c, d, e);
    object->methods = &data_020aede4;
    return object;
}
