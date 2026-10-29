struct Object {
    void *methods;
    unsigned char pad0[48];
    int field52;
};

extern char data_020ac100;
void func_02052074(void *object);

struct Object *func_020520f0(struct Object *object)
{
    func_02052074(object);
    object->methods = &data_020ac100;
    object->field52 = 0;
    return object;
}
