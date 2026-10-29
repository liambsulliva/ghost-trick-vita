struct Object {
    void *methods;
};

extern char data_020b0c60;
void func_02011774(void *object);

struct Object *func_02067150(struct Object *object)
{
    func_02011774(object);
    object->methods = &data_020b0c60;
    *(int *)((char *)object + 216) = 0;
    *(int *)((char *)object + 220) = 0;
    return object;
}
