struct Object {
    void *methods;
};

extern char data_020a2cb8;
void func_0200b504(void *object);
void func_0200b180(void *object);
void func_02007ab4(void *object);
void func_02031270(void *object);

struct Object *func_0200b4b8(struct Object *object)
{
    object->methods = &data_020a2cb8;
    func_0200b504(object);
    func_0200b180((char *)object + 20);
    func_02007ab4(object);
    func_02031270(object);
    return object;
}
