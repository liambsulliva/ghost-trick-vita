struct WithMethods {
    void *methods;
};

extern char data_020a57f4;
void func_0201edc0(void *object, int a, int b, int c, int d, int e);

struct WithMethods *func_02034140(struct WithMethods *object, int a, int b, int c, int d, int e)
{
    func_0201edc0(object, a, b, c, d, e);
    object->methods = &data_020a57f4;
    return object;
}
