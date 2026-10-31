struct Object {
    void *methods;
    unsigned char flag0 : 1;
    unsigned char flag1 : 1;
    unsigned char flag2 : 1;
    unsigned char byte5;
    unsigned char byte6;
    unsigned char byte7;
    short half8;
    unsigned char pad[10];
    int word14;
    unsigned char sub18[8];
    unsigned char sub20[8];
    unsigned char sub28[0x48];
    int word70;
    int word74;
    int word78;
    int word7c;
    int word80;
};

extern char data_020a50b0;
extern struct Object *func_02007aa8(struct Object *object);
extern void func_0202b344(unsigned char *param_1);
extern void func_0202b358(unsigned char *param_1);

struct Object *func_0202b2e0(struct Object *object)
{
    func_02007aa8(object);
    object->methods = &data_020a50b0;
    object->flag0 = 1;
    object->flag1 = 0;
    object->flag2 = 0;
    object->byte6 = 1;
    object->half8 = -2;
    object->word14 = 0;
    func_0202b344(object->sub18);
    func_0202b344(object->sub20);
    object->word70 = 0;
    object->word74 = 0;
    object->word78 = 0;
    object->word7c = 0;
    object->word80 = 0;
    func_0202b358(object->sub28);
    return object;
}
