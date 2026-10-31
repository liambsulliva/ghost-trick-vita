extern void *func_02008444(void);
extern int func_0203435c(void *object);
extern int func_02010b54(unsigned char *object);
extern int func_0202c358(unsigned char *object);
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern void func_02061fc4(void);
extern int func_0200f5c8(void *object);
extern int func_0200f5e8(void *object);
extern void func_02061fd0(int a, int b, int c);
extern void func_020571e4(unsigned char *object);
extern void func_02061fe0(int a);
extern void func_02061fec(int a);
extern void func_02061ff8(int a, int b);
extern void func_02062018(short a, short b, int c);
extern void func_02062034(void);

void func_02061e90(unsigned char *p)
{
    int base;
    int left;
    int top;
    int right;
    int bottom;
    int x;
    int y;

    if (func_0203435c(func_02008444()) == 0 && p[0xc] == 3 && func_02010b54(p) != 0 &&
        func_0202c358(p) != 0 && *(signed char *)(p + 0x44) >= 1) {
        base = func_02029708(func_0202970c());
        func_02061fc4();
        x = *(int *)(p + 0x28);
        y = *(int *)(p + 0x2c);
        if (p[0x7c] == 0) {
            x = x - func_0200f5c8(func_02008444()) - 0x80000;
            y = -y + func_0200f5e8(func_02008444()) + 0x60000;
        }
        left = (*(int *)(p + 0x80) + x) >> 7;
        top = (*(int *)(p + 0x84) + y) / 3 >> 5;
        right = (*(int *)(p + 0x88) + x) >> 7;
        bottom = (*(int *)(p + 0x8c) + y) / 3 >> 5;
        func_02061fd0(0, 0, -(base - *(unsigned short *)(p + 0x7e)));
        func_020571e4(p);
        func_02061fe0(2);
        func_02061fec(0);
        func_02061ff8(0, 0);
        func_02062018(left, top, 0);
        func_02061ff8(0, 0x10000);
        func_02062018(left, bottom, 0);
        func_02061ff8(0x10000, 0x10000);
        func_02062018(right, top, 0);
        func_02061ff8(0x10000, 0);
        func_02062018(right, bottom, 0);
        func_02062034();
    }
}
