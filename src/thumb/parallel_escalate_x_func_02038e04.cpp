class Badge {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
};

typedef int code(...);

extern "C" {
extern int data_020a690c;
extern unsigned char data_0209a76c[][2];
extern void func_020370f0(void *object);
extern void *func_02008444(void);
extern int func_0201ff10(void *a, int b);
extern int func_0201fbc8(int a);
extern unsigned char func_02037130(unsigned char a);
extern int *func_0202f624(int size);
extern int *func_02038df0(int *p);
extern void func_0205765c(int *p, int a, int b, int c, int d);
extern void func_0203a338(int *p);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, int *b, int c, int d);
extern void func_02039348(void *p);
extern void func_0203936c(void *p);
extern int func_0201f944(int a, unsigned char b);
extern void func_0201f974(int a, unsigned char b, unsigned char c, int d);

void *func_02038e04(Badge *badge, unsigned char kind, unsigned char style, unsigned char quiet);
}

void *func_02038e04(Badge *badge, unsigned char kind, unsigned char style, unsigned char quiet)
{
    unsigned char *self;

    int *p;
    int save;
    int list;
    int level;
    unsigned char i;

    self = (unsigned char *)badge;
    func_020370f0(self);
    *(int *)self = (int)&data_020a690c;
    *(int *)(self + 0xcc) = 0;
    *(int *)(self + 0xd0) = 0;
    *(int *)(self + 0xd4) = 0;
    *(int *)(self + 0xd8) = 0;
    self[0xdc] = kind;
    self[0xdd] = 0;
    self[0xde] = style;
    self[0xdf] = 0;
    self[0xe0] = 0;
    self[0xe1] = 0;
    self[0xe2] = 0;
    self[0xe3] = 0;
    self[0xe4] = quiet;
    self[0xe5] = 0;
    save = func_0201ff10(func_02008444(), 0);
    if (save != 0) {
        *(int *)(self + 0xd8) = func_0201fbc8(save);
        self[0xdd] = func_02037130(self[0xdc]);
    }
    p = func_0202f624(0xd8);
    if (p != 0) {
        p = func_02038df0(p);
    }
    *(int **)(self + 0xcc) = p;
    if (p != 0) {
        (**(code **)(*p + 0xa8))(p, 0x59, 0, 1, 0, 1, -1, 0);
        (**(code **)(**(int **)(self + 0xcc) + 0x84))(*(int **)(self + 0xcc), 0x480);
        func_0205765c(*(int **)(self + 0xcc), 0, 0, 0x80000, 0x8000);
        (**(code **)(**(int **)(self + 0xcc) + 0xb8))(*(int **)(self + 0xcc), 0x80000, 0x8000);
        (**(code **)(**(int **)(self + 0xcc) + 0x34))(*(int **)(self + 0xcc), 0x10000, 0xc000, 0);
        func_0203a338(*(int **)(self + 0xcc));
        func_0202f7e8(func_02009900(), *(int **)(self + 0xcc), 1, 0);
    }
    *(int *)(self + 0xb0) -= 0x2c000;
    *(int *)(self + 0xb4) -= 0xc000;
    func_02039348(self);
    func_0203936c(self);
    badge->s16();
    if (quiet != 0 || self[0xdd] == -1) {
        return self;
    }
    {
        list = func_0201ff10(func_02008444(), 2);
        if (list != 0) {
            level = func_0201f944(list, self[0xdd]);
            for (i = 0; i < 2; i++) {
                if (data_0209a76c[self[0xdd]][i] != -1 && level >= data_0209a76c[self[0xdd]][i]) {
                    func_0201f974(save, self[0xdd], (unsigned char)((i + 1) * 10), -1);
                    self[0xe2] = i + 1;
                }
            }
        }
    }
    return self;
}
