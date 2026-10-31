struct Object {
    void *table;
};
struct Input {
    unsigned int first;
    int second;
    int third;
};
extern char data_020a2ecc;
extern void func_02007aa8(struct Object *object);
extern void func_0200d80c(struct Object *object,int value);
extern void func_0200d818(struct Object *object,int value);
struct Object *func_0200d824(struct Object *object,const struct Input *input)
{
    func_02007aa8(object);
    object->table = &data_020a2ecc;
    func_0200d80c(object,input->second);
    func_0200d818(object,input->third);
    return object;
}
