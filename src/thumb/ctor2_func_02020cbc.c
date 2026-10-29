struct Object {
    void *methods;
};

extern char data_020a490c;
void func_02020d40(void *object);
void func_02031270(void *object);

struct Object *func_02020cbc(struct Object *object)
{
    object->methods = &data_020a490c;
    func_02020d40(object);
    func_02031270(object);
    return object;
}
