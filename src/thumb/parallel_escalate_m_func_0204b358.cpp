class Obj0204b358A
{
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(int argument0); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(int argument0, int argument1, int argument2); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(int argument0);
};

extern "C" {
unsigned int *func_02008444(void);
int func_0200f330(unsigned int *argument0);
void func_02056cb8(unsigned char *argument0);
int func_0200f5c8(unsigned int *argument0);
int func_0200f5e8(unsigned int *argument0);

void func_0204b358(unsigned char *param_1)
{
    int iVar1;

    iVar1 = func_0200f330(func_02008444());
    if (iVar1 == 0) {
        func_02056cb8(param_1);
        *(int *)(param_1 + 0xdc) = *(int *)(param_1 + 0xdc) + *(int *)(param_1 + 0xc8);
        *(int *)(param_1 + 0xe0) = *(int *)(param_1 + 0xe0) - *(int *)(param_1 + 0xcc);
        *(int *)(param_1 + 0xcc) = *(int *)(param_1 + 0xcc) + *(int *)(param_1 + 0xd0);
        ((Obj0204b358A *)param_1)->s35((signed char)(*(int *)(param_1 + 0xd4) >> 12));
        ((Obj0204b358A *)param_1)->s22(*(int *)(param_1 + 0xd4) >> 7, *(int *)(param_1 + 0xd4) >> 7, 0x1000);
        iVar1 = func_0200f5c8(func_02008444());
        *(int *)(param_1 + 0x28) = (*(int *)(param_1 + 0xdc) - iVar1) + -0x80000;
        *(int *)(param_1 + 0x2c) = -*(int *)(param_1 + 0xe0) + func_0200f5e8(func_02008444()) + 0x60000;
        iVar1 = *(int *)(param_1 + 0xd4);
        *(int *)(param_1 + 0xd4) = iVar1 - *(int *)(param_1 + 0xd8);
        if (iVar1 - *(int *)(param_1 + 0xd8) < 0) {
            ((Obj0204b358A *)param_1)->s8(0);
        }
    }
}
}
