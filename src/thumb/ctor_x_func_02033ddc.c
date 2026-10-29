struct Object {
    void *methods;
};

extern char data_020a5870;
void func_02022d70(void *object);
void func_0200f70c(void *object);

struct Object *func_02033ddc(struct Object *object)
{
    func_02022d70(object);
    object->methods = &data_020a5870;
    *(int *)((char *)object + 32) = 0;
    func_0200f70c((char *)object + 52);
    return object;
}
