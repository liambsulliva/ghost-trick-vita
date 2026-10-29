struct Object {
    void *methods;
};

extern char data_020af2c8;
void func_02056ac8(void *object);

struct Object *func_0205e4b0(struct Object *object)
{
    func_02056ac8(object);
    object->methods = &data_020af2c8;
    *(int *)((char *)object + 200) = 0;
    *(int *)((char *)object + 204) = 0;
    return object;
}
