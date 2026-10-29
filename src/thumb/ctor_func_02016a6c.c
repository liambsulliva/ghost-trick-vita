struct Object {
    void *methods;
    unsigned char pad0[8];
    int field12;
};

extern char data_020a36d0;
void func_020164f0(void *object);

struct Object *func_02016a6c(struct Object *object)
{
    func_020164f0(object);
    object->methods = &data_020a36d0;
    object->field12 = 0;
    return object;
}
