struct Object {
    void *methods;
};

extern char data_020a5440;
void func_020153d8(void *object, int value);
void func_02007ab4(void *object);

struct Object *func_0202f5a4(struct Object *object)
{
    object->methods = &data_020a5440;
    func_020153d8(object, 0);
    func_02007ab4(object);
    return object;
}
