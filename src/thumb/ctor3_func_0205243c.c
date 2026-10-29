struct Object {
    void *methods;
};

extern char data_020ac148;
void func_020520b0(void *object);
void func_0202f5e4(void *object);
void func_0202f644(void *object);

struct Object *func_0205243c(struct Object *object)
{
    object->methods = &data_020ac148;
    func_020520b0(object);
    func_0202f5e4(object);
    func_0202f644(object);
    return object;
}
