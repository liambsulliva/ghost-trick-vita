struct Base {
    void *vtable;
    unsigned char pad004[0x104];
    unsigned short h108;
    signed char b10a;
    unsigned char b10b;
    unsigned short h10c;
    unsigned char pad10e[6];
    int w114;
    signed char b118;
    unsigned char pad119;
    unsigned char b11a;
    unsigned char b11b;
    unsigned char b11c;
    unsigned char pad11d[3];
    int w120;
    int w124;
    int w128;
    int w12c;
};

extern void func_020444b0(struct Base *object);
extern char data_020a88e4;

struct Base *func_02044ef4(struct Base *param_1)
{
    func_020444b0(param_1);
    param_1->vtable = &data_020a88e4;
    param_1->h108 = 0;
    param_1->b10a = -1;
    param_1->b10b = 0;
    param_1->h10c = 0;
    param_1->w114 = 0;
    param_1->b118 = -1;
    param_1->b11a = 0;
    param_1->b11b = 0;
    param_1->b11c = 0;
    param_1->w120 = 0x1000;
    param_1->w124 = 0x1000000;
    param_1->w128 = 0;
    param_1->w12c = -1;
    return param_1;
}
