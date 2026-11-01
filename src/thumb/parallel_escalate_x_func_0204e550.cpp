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

struct Owner {
    unsigned char pad0[0x28];
    int layer;
};

extern "C" {
extern char data_020cf724[];
extern void func_0202f64c(Item **a);
extern Item *func_0205693c(char *a);
extern void func_020153d8(Item *a, struct Owner *b);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, Item *b, int c, int d);
extern void func_0205765c(Item *a, int b, int c, int d, int e);
void func_0204e550(struct Owner *o, Item **a, Item **b, int *len, int *step, unsigned short kind, unsigned short size);
}

void func_0204e550(struct Owner *o, Item **a, Item **b, int *len, int *step, unsigned short kind, unsigned short size)
{
    int end;

    if (*a == 0) {
        return;
    }
    *len += *step;
    if (*len > 0x180000) {
        *len -= 0x180000;
        (*a)->s46(0x40000, 0xc0000);
        (*a)->s13(o->layer, 0, 0);
        func_0202f64c(b);
    }
    end = *len + 0xc0000;
    if (end > 0x180000) {
        if (*b == 0) {
            func_020153d8(*b = func_0205693c(data_020cf724), o);
            (*b)->s42(kind, 0, 1, 0, 1, -1, 0);
            (*b)->s33(size);
            func_0202f7e8(func_02009900(), *b, 7, 0);
        }
        func_0205765c(*a, 0, *len, 0x40000, 0x180000);
        (*a)->s46(0x40000, 0x180000 - *len);
        (*a)->s13(o->layer, (*len >> 1) - 0x60000, 0);
        func_0205765c(*b, 0, 0, 0x40000, end - 0x180000);
        (*b)->s46(0x40000, end - 0x180000);
        (*b)->s13(o->layer, (end >> 1) - 0x120000, 0);
    } else {
        func_0205765c(*a, 0, *len, 0x40000, end);
    }
}
