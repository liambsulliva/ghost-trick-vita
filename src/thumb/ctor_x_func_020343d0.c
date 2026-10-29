struct Object {
    void *methods;
};

extern char data_020a5be8;
void func_02031d20(void *object);
void func_0200f70c(void *object);

struct Object *func_020343d0(struct Object *object)
{
    func_02031d20(object);
    object->methods = &data_020a5be8;
    func_0200f70c((char *)object + 64);
    return object;
}
