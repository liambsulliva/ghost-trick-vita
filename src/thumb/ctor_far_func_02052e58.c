struct Object {
    void *methods;
};

extern char data_020aca14;
void func_02052cec(void *object);

struct Object *func_02052e58(struct Object *object)
{
    func_02052cec(object);
    object->methods = &data_020aca14;
    ((char *)object)[85 << 2] = 0;
    return object;
}
