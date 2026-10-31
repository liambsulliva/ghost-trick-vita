class Obj02063cac {
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
};

extern "C" {
void func_0207193c(int value);
void func_02063d64(int a, int b, int c, int d, int e, int f, int g);
void func_02071dbc(void);
void func_020716e8(int value);
unsigned int *func_02008444(void);
void func_020322b4(unsigned int *object);
int func_02049bdc(unsigned int *object);
int func_02049be8(unsigned int *object);

void func_02063cac(unsigned char *p)
{
    if (p[0xc5] == 0) {
        func_0207193c(8);
        func_02063d64(3, 0, 0, 0, 3, 0x10, 0);
        p[0xc5] = 1;
    } else {
        if (p[0xc6] == 0) {
            if ((*(volatile unsigned int *)0x04000064 & 0x80000000) != 0) {
                return;
            }
            ((Obj02063cac *)p)->s55();
            func_02071dbc();
            func_020716e8(0xc);
            p[0xc6] = 1;
            if (p[0xc8] != 0) {
                p[0xc8] = 0;
                func_020322b4(func_02008444());
            }
        }
        if (p[0xc5] != 0 && p[0xc6] != 0 && p[0xc9] != 0) {
            *(int *)(p + 0x28) = -func_02049bdc(func_02008444());
            *(int *)(p + 0x2c) = func_02049be8(func_02008444());
        }
    }
}
}
