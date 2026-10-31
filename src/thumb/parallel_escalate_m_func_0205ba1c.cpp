class Obj0205ba1cA
{
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(int argument0, int argument1, int argument2);
};

extern "C" {
int func_0205b830(unsigned char *argument0);
int func_0205b838(unsigned char *argument0);
int func_0205b850(unsigned char *argument0);
int func_0205b858(unsigned char *argument0);

void func_0205ba1c(unsigned char *param_1, unsigned char param_2)
{
    int iVar1;
    int iVar2;

    param_1[0x9c] = param_2;
    iVar1 = func_0205b830(param_1);
    iVar2 = func_0205b838(param_1);
    ((Obj0205ba1cA *)*(int *)(param_1 + 0x28))->s13(iVar1 << 12, iVar2 << 12, 0);
    iVar1 = func_0205b850(param_1);
    iVar2 = func_0205b858(param_1);
    ((Obj0205ba1cA *)*(int *)(param_1 + 0x2c))->s13(iVar1 << 12, iVar2 << 12, 0);
    if (*(int *)(param_1 + 0x30) != 0) {
        iVar1 = func_0205b830(param_1);
        iVar2 = func_0205b838(param_1);
        ((Obj0205ba1cA *)*(int *)(param_1 + 0x30))->s13(iVar1 << 12, iVar2 << 12, 0);
    }
    if (*(signed char *)(param_1 + 0x9c) > 3 || *(signed char *)(param_1 + 0x9c) < 0) {
        ((Obj0205ba1cA *)*(int *)(param_1 + 0x28))->s11();
        ((Obj0205ba1cA *)*(int *)(param_1 + 0x2c))->s11();
        if (*(int *)(param_1 + 0x30) != 0) {
            ((Obj0205ba1cA *)*(int *)(param_1 + 0x30))->s11();
            return;
        }
    }
    else {
        ((Obj0205ba1cA *)*(int *)(param_1 + 0x28))->s10();
        ((Obj0205ba1cA *)*(int *)(param_1 + 0x2c))->s10();
        if (*(int *)(param_1 + 0x30) != 0) {
            ((Obj0205ba1cA *)*(int *)(param_1 + 0x30))->s10();
        }
    }
}
}
