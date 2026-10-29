struct Object {
    void *methods;
};

extern char data_020a7268;
void func_02022d70(void *object);
void func_0200f70c(void *object);

struct Object *func_0203dbac(struct Object *object)
{
    func_02022d70(object);
    object->methods = &data_020a7268;
    func_0200f70c((char *)object + 44);
    return object;
}
