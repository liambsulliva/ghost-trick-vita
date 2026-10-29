struct Object {
    void *methods;
};

extern char data_020afed8;
void func_02056ac8(void *object);

struct Object *func_02063bc0(struct Object *object)
{
    func_02056ac8(object);
    object->methods = &data_020afed8;
    *((char *)object + 197) = 0;
    *((char *)object + 198) = 0;
    *((char *)object + 200) = 0;
    *((char *)object + 201) = 0;
    return object;
}
