class Obj3afdc {
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
    virtual void s33(int a);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(unsigned short id, int a, int b, int c, int d, int e, int f);
};
extern "C" {
extern int *func_02031254(int size);
extern int *func_0203b238(int *p, int kind, int *owner);
extern void func_02038ac4(int *p);
extern void func_02038acc(int p, int v);
extern unsigned long long func_0203b2c0(int p, unsigned int a, unsigned int b);
extern int func_02010b7c(unsigned long long v);
extern void func_0202f174(int a, int b);

void func_0203afdc(int *param_1)
{
    int *piVar1;
    int iRes;

    ((Obj3afdc *)param_1)->s33(0);
    if (param_1[0x3a] == 0) {
        if (param_1[0x32] == 1) {
            param_1[0x3a] = 0x52;
        } else if (param_1[0x32] == 2) {
            param_1[0x3a] = 0x54;
        }
    }
    ((Obj3afdc *)param_1)->s42((unsigned short)param_1[0x3a], 0, 1, 0, 1, -1, 0);
    param_1[10] = param_1[0x38];
    param_1[0xb] = param_1[0x39];
    if (param_1[0x32] != 0) {
        piVar1 = func_02031254(0x18);
        if (piVar1 != 0) {
            piVar1 = func_0203b238(piVar1, param_1[0x32], param_1);
        }
        param_1[0x33] = (int)piVar1;
        if (piVar1 != 0) {
            func_02038ac4(piVar1);
            func_02038acc(param_1[0x33], 1);
            iRes = func_02010b7c(func_0203b2c0(param_1[0x33], *(unsigned short *)((char *)param_1 + 0xd4), *(unsigned short *)((char *)param_1 + 0xd6)));
            func_0202f174(iRes, param_1[0x33]);
        }
    }
}
}
