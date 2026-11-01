class Item {
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
    virtual void s13(int x, int y, int z);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33(int a);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int a, int b, int c, int d, int e, int f, int g);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int a, int b);
};

struct Menu {
    unsigned char pad0[0x28];
    Item *frame;
    Item *items[4];
    int depth;
};

extern "C" {
extern void func_020467fc(Item *a, int b);
extern void func_0204681c(Item *a, int b);
extern void func_0203ce3c(Item *a, int b, int c);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, Item *b, int c, int d);
extern void func_0205765c(Item *a, int b, int c, int d, int e);
void func_02046920(struct Menu *p);
}

void func_02046920(struct Menu *p)
{
    unsigned int i;

    func_020467fc(p->frame, 0x470);
    func_0204681c(p->frame, 1);
    p->frame->s10();
    func_0203ce3c(p->frame, 0, 0x5a);
    func_0203ce3c(p->frame, 1, -0x5a);
    p->depth = 0x4e;
    func_0202f7e8(func_02009900(), p->frame, 7, 0);
    for (i = 0; i < 4; i++) {
        p->items[i]->s42(0x11, 0, 1, 1, 0, -1, 0);
        p->items[i]->s46(0x10000, 0xc000);
        p->items[i]->s33(0x460);
        p->items[i]->s11();
        func_0202f7e8(func_02009900(), p->items[i], 7, 0);
    }
    func_0205765c(p->items[0], 0x10000, 0, 0, 0xc000);
    p->items[0]->s13(-0x7f000, p->depth << 12, 0);
    func_0205765c(p->items[1], 0, 0, 0x10000, 0xc000);
    p->items[1]->s13(0x7e000, p->depth << 12, 0);
    func_0205765c(p->items[2], 0x10000, 0xc000, 0, 0);
    p->items[2]->s13(-0x7f000, -p->depth << 12, 0);
    func_0205765c(p->items[3], 0, 0xc000, 0x10000, 0);
    p->items[3]->s13(0x7e000, -p->depth << 12, 0);
}
