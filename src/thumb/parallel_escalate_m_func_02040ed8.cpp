class Obj {
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
};
extern "C" {
extern void func_0200b518(void *obj, int a, unsigned int b, unsigned int c);
extern void func_0200b598(void *obj, int a, int b);
extern void func_020280a8(void *obj, int v);
extern int func_02008444(void);
extern int func_020322d4(int v);
extern void func_02040e78(unsigned char *p);
extern void func_02040ea8(unsigned char *p);

void func_02040ed8(unsigned char *p)
{
    func_0200b518(p + 0x28, 1, 0x80000000, 0x80000000);
    if (*(int *)(p + 0xbc) != 0) {
        func_0200b598(p + 0x28, *(int *)(p + 0xbc), 15);
        func_020280a8(p + 0x28, p[0xc0]);
        ((Obj *)p)->s10();
    }
    p[0xe0] = func_020322d4(func_02008444());
    if (p[0xe0] != 0) {
        *(int *)(p + 0xd4) = 9 << 14;
    } else {
        *(int *)(p + 0xd4) = 11 << 14;
    }
    *(int *)(p + 0xd8) = 0x18000;
    *(int *)(p + 0xcc) = *(int *)(p + 0xb0);
    *(int *)(p + 0xd0) = *(int *)(p + 0xb4);
    if (*(int *)(p + 0xdc) != 0x120000) {
        *(int *)(p + 0xdc) = *(int *)(p + 0xd4);
    }
    *(int *)(p + 0xb0) = *(int *)(p + 0xb0) + *(int *)(p + 0xdc);
    *(int *)(p + 0xb4) = *(int *)(p + 0xb4) + *(int *)(p + 0xd8);
    if (*(int *)(p + 0xbc) == 0x12f1) {
        ((Obj *)p)->s11();
    }
    if (p[0xe0] != 0) {
        func_02040e78(p);
    } else {
        func_02040ea8(p);
    }
    p[0x10] = 0;
}
}
