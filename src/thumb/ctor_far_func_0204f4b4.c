struct Object {
    void *methods;
};

extern char data_020ab740;
void func_02056ac8(void *object);

struct Object *func_0204f4b4(struct Object *object)
{
    func_02056ac8(object);
    object->methods = &data_020ab740;
    *((char *)object + 197) = 255;
    *((char *)object + 198) = 255;
    return object;
}
