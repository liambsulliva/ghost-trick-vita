struct Object {
    void *methods;
};

extern char data_020a7a7c;
void func_020404ac(void *object);

struct Object *func_020401a8(struct Object *object)
{
    func_020404ac(object);
    object->methods = &data_020a7a7c;
    *((char *)object + 89) = 1;
    return object;
}
