struct Object {
    void *methods;
};

extern char data_020a2ecc;
void func_02007aa8(void *object);
extern char data_02083be0;
extern char data_02083bdc;

struct Object *func_0200d7e4(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a2ecc;
    *(void **)((char *)object + 4) = &data_02083be0;
    *(void **)((char *)object + 8) = &data_02083bdc;
    *(int *)((char *)object + 12) = 0;
    return object;
}
