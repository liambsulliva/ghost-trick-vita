struct Object {
    void *table;
    int f4;
    int f8;
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 4;
    unsigned char fd;
    unsigned short pad;
    int f10;
    int f14;
    int f18;
    int f1c;
    int f20;
    int f24;
};
extern char data_020a2cd4;
extern void func_02007aa8(struct Object *object);

struct Object *func_0200ae2c(struct Object *object)
{
    func_02007aa8(object);
    object->table = &data_020a2cd4;
    object->b0 = 0;
    object->b1 = 0;
    object->b2 = 0;
    object->b3 = 0;
    object->b4 = 0;
    object->f18 = 0x80000000;
    object->f1c = 0x80000000;
    object->f20 = 0x1000;
    object->f24 = 0;
    object->f8 = -1;
    object->f4 = -1;
    object->fd = 0xff;
    return object;
}
