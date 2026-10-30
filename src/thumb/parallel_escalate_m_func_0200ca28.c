struct Bits {
    unsigned int low : 24;
    unsigned int middle : 7;
    unsigned int top : 1;
};
struct Object {
    void *table;
    int count;
    struct Bits bits;
    int value;
    int list;
};
extern char data_020a4a14;
extern void func_02007aa8(struct Object *object);
extern void func_0200ca60(int *list,int value);
struct Object *func_0200ca28(struct Object *object)
{
    func_02007aa8(object);
    object->table = &data_020a4a14;
    object->count = 0;
    object->bits.low = 0;
    object->bits.top = 0;
    object->value = 0;
    func_0200ca60(&object->list,0);
    object->list = 0;
    return object;
}
