class Sprite48f9c {
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
    virtual void s13(int a, int b, int c);
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
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int a, int b);
};

class Panel48f9c {
public:
    char pad0[0xb8];
    Sprite48f9c *sprite;
    int width;
    char padc0[4];
    unsigned char wide;
};

extern "C" {
extern void func_0205765c(Sprite48f9c *a, int b, int c, int d, int e);
void func_02048f9c(Panel48f9c *p);
}

void func_02048f9c(Panel48f9c *p)
{
    if (p->wide != 0) {
        p->sprite->s46(0x80000, p->width << 1);
        p->sprite->s13(-0x40000, 0x60000 - ((p->width << 1) >> 1), 0);
    } else {
        p->sprite->s46(0x100000, p->width << 1);
        p->sprite->s13(0, 0x60000 - ((p->width << 1) >> 1), 0);
    }
    func_0205765c(p->sprite, 0, 0x40000 - (p->width >> 1), 0x100000, 0x40000);
}
