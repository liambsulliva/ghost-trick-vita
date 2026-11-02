class Panel {
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
    virtual void s22(int a, int b, int c);
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
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68(Panel *a, char *b, int c);
    unsigned char pad4[0xd8];
    void *owner;
    unsigned char lowe0 : 2;
    unsigned char shown : 1;
    unsigned char highe0 : 5;
    unsigned char pade1[0x4f];
    unsigned char low130 : 5;
    unsigned char mode : 3;
    unsigned char pad131[0x27];
    int effect;
    int pad15c;
    int handle;
};

extern "C" {
extern char data_0209b594[];
extern char data_0209b618[];
extern char data_0209b644[];
extern char data_0209b654[];
extern char data_0209b634[];
extern void func_02010b74(Panel *p);
extern void *func_02008444(void);
extern int func_02031af4(void *a, int b);
extern void func_02031b04(void *a, int *b);
extern void func_0202f64c(int *a);
extern void func_0200da04(char *a, char *b, char *c, char *d);
void func_020461b8(Panel *p);
}

void func_020461b8(Panel *p)
{
    int handle;

    p->mode = 3;
    p->shown = 1;
    if (p->owner == 0) {
        func_02010b74(p);
        return;
    }
    p->s68(p, data_0209b594, 6);
    p->s13(0, 0x4e000, 0);
    p->s22(0x1000, 0x1000, 0x1000);
    handle = p->handle;
    if (handle >= 0 && func_02031af4(func_02008444(), handle) == 0 && p->effect != 0) {
        func_0202f64c(&p->effect);
        func_02031b04(func_02008444(), &p->handle);
    }
    func_0200da04(data_0209b618, data_0209b644, data_0209b654, data_0209b634);
}
