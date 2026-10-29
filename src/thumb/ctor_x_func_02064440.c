struct Object {
    void *methods;
};

extern char data_020b035c;
void func_0202f524(void *object);

struct Object *func_02064440(struct Object *object)
{
    func_0202f524(object);
    object->methods = &data_020b035c;
    *(int *)((char *)object + 40) = 0;
    *(int *)((char *)object + 44) = 0;
    *(int *)((char *)object + 48) = 4096;
    *(int *)((char *)object + 52) = 4096;
    return object;
}
