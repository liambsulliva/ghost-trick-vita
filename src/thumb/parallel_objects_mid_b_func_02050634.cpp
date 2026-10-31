class Obj02050634A
{
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(int argument0);
    virtual void s9(); virtual void s10(); virtual void s11();
};

extern "C" {
int *func_02008444();
void func_0200b598(unsigned char *argument0, int argument1, int argument2);
void func_0200b7c0(unsigned char *argument0);
void func_0201134c(int *argument0);
int func_0202657c(int *argument0);
int func_0202671c(unsigned char *argument0);
void func_020297f0(int *argument0, int argument1);

void func_02050634(unsigned char *param_1)
{
    switch (param_1[0x10]) {
    case 0:
        if (*(int *)(param_1 + 0xbc) != 0) {
            func_0200b598(param_1 + 0x28, *(int *)(param_1 + 0xbc), 0xf);
            func_0200b7c0(param_1 + 0x28);
            ((Obj02050634A *)param_1)->s10();
            *(int *)(param_1 + 0xbc) = 0;
            param_1[0x10] = param_1[0x10] + 1;
        }
        return;
    case 1:
        if (*(int *)(param_1 + 0xbc) != 0) {
            func_0200b598(param_1 + 0x28, *(int *)(param_1 + 0xbc), 0xf);
            *(int *)(param_1 + 0xbc) = 0;
        }
        func_0200b7c0(param_1 + 0x28);
        if (param_1[0x13] != 0) {
            if (func_0202657c(func_02008444()) != 0) {
                ((Obj02050634A *)param_1)->s11();
            }
        }
        if (param_1[0xc0] != 0 && func_0202671c(param_1 + 0x28) != 0) {
            if (param_1[0x13] != 0) {
                func_0201134c(func_02008444());
            }
            func_020297f0(func_02008444(), *(int *)(param_1 + 0xc4));
            ((Obj02050634A *)param_1)->s8(0);
        }
        return;
    default:
        return;
    }
}
}
