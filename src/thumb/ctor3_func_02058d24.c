struct Object {
    void *methods;
};

extern char data_020ae600;
void func_020590c0(void *object);
void func_02033f44(void *object);
void func_0202f644(void *object);

struct Object *func_02058d24(struct Object *object)
{
    object->methods = &data_020ae600;
    func_020590c0(object);
    func_02033f44(object);
    func_0202f644(object);
    return object;
}
