struct Owner {
    void *vtable;
    unsigned char pad4[0x10 - 4];
    void *model;
    int f14;
    int f18;
    unsigned char pad1c[0x20 - 0x1c];
    unsigned short row;
    unsigned char pad22[0x2a - 0x22];
    unsigned short h2a;
    unsigned char pad2c[0x30 - 0x2c];
    short h30;
    short h32;
    unsigned char pad34[0x40 - 0x34];
    short h40;
    unsigned short h42;
    unsigned short h44;
    unsigned short h46;
    unsigned short h48;
    unsigned short h4a;
    unsigned short h4c;
    unsigned char pad4e[0x60 - 0x4e];
    int shadow;
    unsigned char pad64[0x68 - 0x64];
    int f68;
    int f6c;
    unsigned char b70;
    unsigned char b71;
    unsigned char b72;
    signed char b73;
    unsigned char b74;
};

extern const char data_02084a8c[];
extern const char data_02084a8e[];
extern const char data_02084a94[];
extern char data_020b9df4[];
extern void func_0201778c(struct Owner *a);
extern void func_02017714(struct Owner *a, int b);
extern unsigned char func_02019cec(struct Owner *a, int b);
extern void func_0201e014(int a);
extern int func_0201dfe4(int a, unsigned char b);
extern void func_0201e02c(int a, unsigned char b, unsigned char c);
extern int func_020198cc(int a);
extern void func_0201e04c(int a, unsigned char b, unsigned short c, int d);
extern void func_0201c6c4(char *a, void *model);
extern void func_0201cf2c(char *a, int shadow);
extern void func_02016fbc(char *a, int b);
extern void func_02017110(struct Owner *a, int b);
extern void func_02016e28(struct Owner *a);

void func_020197b4(struct Owner *param_1, unsigned short param_2, unsigned char param_3, unsigned char param_4, int param_5, unsigned char param_6)
{
    int idx;
    int row;

    param_1->row = 3;
    func_0201778c(param_1);
    func_02017714(param_1, 0);
    param_1->h2a = 3;
    row = param_1->row;
    param_1->f18 = param_5;
    idx = row * 0x12;
    param_1->f14 = param_5;
    param_1->h30 = *(short *)(data_02084a8c + idx);
    param_1->h32 = *(short *)(data_02084a8e + idx);
    param_1->h42 = param_2;
    param_1->b73 = param_3;
    param_1->h44 = param_1->b73;
    param_1->h40 = *(short *)(data_02084a94 + idx) - 1;
    param_1->b74 = 0;
    param_1->b71 = func_02019cec(param_1, param_5);
    func_0201e014(param_1->f6c);
    if (func_0201dfe4(param_1->f6c, param_1->b71) != 0) {
        func_0201e02c(param_1->f6c, param_1->b70, param_6);
        param_1->f68 = func_020198cc(param_1->f6c);
        param_1->b70 = 0;
        param_1->b72 = param_4;
        func_0201e04c(param_1->f6c, param_1->b70, param_1->h44, 0);
        param_1->h46 = 0x100;
        param_1->h48 = 0xc0;
        param_1->h4a = param_1->h46;
        param_1->h4c = param_1->h46;
        func_0201c6c4(data_020b9df4, param_1->model);
        func_0201cf2c(data_020b9df4, param_1->shadow);
        func_02016fbc(data_020b9df4, 3);
        func_02017110(param_1, param_1->f14);
        func_02016e28(param_1);
    }
}
