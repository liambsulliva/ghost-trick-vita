struct Object {
    void *methods;
};

extern char data_020b163c;
void func_02056ac8(void *object);

struct Object *func_02067a84(struct Object *object)
{
    func_02056ac8(object);
    object->methods = &data_020b163c;
    *(int *)((char *)object + 200) = 0;
    *(int *)((char *)object + 204) = 0;
    *(int *)((char *)object + 208) = 0;
    return object;
}
