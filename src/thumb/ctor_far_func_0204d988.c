struct Object {
    void *methods;
};

extern char data_020ab04c;
void func_0204c27c(void *object);

struct Object *func_0204d988(struct Object *object)
{
    func_0204c27c(object);
    object->methods = &data_020ab04c;
    *(short *)((char *)object + 196) = 0;
    return object;
}
