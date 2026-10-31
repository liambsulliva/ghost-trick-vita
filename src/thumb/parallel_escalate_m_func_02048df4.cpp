class Obj02048df4 {
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
    virtual void m33(int a);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void m37(int a);
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void m42(int a, int b, int c, int d, int e, int f, int g);
};

extern "C" {
extern void func_0200ee34(char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
extern void func_02048f9c(int *param_1);

void func_02048df4(int *param_1)
{
    int zero;
    int one;

    zero = 0;
    one = 1;
    ((Obj02048df4 *)param_1[0x2e])->m42(9, zero, 1, zero, one, one - 2, zero);
    func_0200ee34((char *)param_1[0x2e], 0x18, 0x18, 0x18);
    ((Obj02048df4 *)param_1[0x2e])->m37(0x21);
    ((Obj02048df4 *)param_1[0x2e])->m33(0xd00);
    if (*(unsigned short *)(param_1 + 0x22) == 0x8005) {
        param_1[0x2f] = one << 18;
    }
    if (*(unsigned short *)(param_1 + 0x22) == 0x8006) {
        param_1[0x2f] = 0x40000;
        *(unsigned char *)(param_1 + 0x31) = 1;
    }
    func_02048f9c(param_1);
    ((Obj02048df4 *)param_1)->m33(0x800);
}
}
