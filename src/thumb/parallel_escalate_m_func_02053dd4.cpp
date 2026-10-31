class Obj02053dd4A
{
public:
    virtual int *s0();
    virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5();
    virtual void s6(); virtual void s7(); virtual void s8(); virtual void s9(); virtual void s10();
    virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20();
    virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25();
    virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40();
    virtual void s41(unsigned char *argument0, int *argument1, int argument2);
    virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(int);
};

extern "C" {
int *func_02008444();
int func_0200f330(int *argument0);
void func_020592c8(unsigned char *argument0);
int func_02031af4(int *argument0, int argument1);
int func_02031b04(int *argument0, unsigned char *argument1);
int func_02010b54(unsigned char *argument0);
int *func_02059b88(unsigned char *argument0);
int func_02008350(unsigned char *argument0);
void func_02029ab8(int *argument0);
void func_020153d8(int *argument0, unsigned char *argument1);
int func_02031b44(int *argument0);
int *func_02009900();
void func_0202f7e8(int *argument0, int *argument1, int argument2, int argument3);

void func_02053dd4(unsigned char *param_1, int *param_2)
{
    int *piVar4;
    int *piVar5;
    int iVar3;
    int iVar6;

    if (func_0200f330(func_02008444()) == 0) {
        func_020592c8(param_1);
    }
    if (param_1[0x10] == 0) {
        if (func_02031af4(func_02008444(), *(int *)(param_1 + 0x158)) != 0) {
            func_02031b04(func_02008444(), param_1 + 0x158);
            param_1[0x10] = 1;
            return;
        }
        if (func_02010b54(param_1) != 0) {
            if (param_1[0x154] == 0) {
                piVar4 = func_02059b88(param_1);
                if (piVar4 != 0) {
                    piVar5 = ((Obj02053dd4A *)param_2)->s0();
                    if (piVar5 != 0) {
                        iVar3 = func_02008350(param_1);
                        func_02029ab8(piVar4);
                        ((Obj02053dd4A *)piVar5)->s41(param_1, piVar4, iVar3);
                        func_020153d8(piVar5, param_1);
                        iVar6 = func_02031b44(func_02008444());
                        *(int *)(param_1 + 0x158) = iVar6;
                        ((Obj02053dd4A *)piVar5)->s46(iVar6);
                        func_0202f7e8(func_02009900(), piVar5, 7, 0);
                        param_1[0x154] = 1;
                    }
                }
            }
        }
    }
}
}
