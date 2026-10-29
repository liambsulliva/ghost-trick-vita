struct Object {
    void *methods;
};

extern char data_020ad698;
void func_0204f490(void *object);

struct Object *func_02056020(struct Object *object)
{
    func_0204f490(object);
    object->methods = &data_020ad698;
    *(int *)((char *)object + 208) = 0;
    return object;
}
