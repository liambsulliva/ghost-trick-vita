class Obj020632f0 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void m6();
};

extern "C" {
extern unsigned int func_0206336c(int *param_1, int *param_2);
extern void func_020083b0(int *dst, int *src);

void func_020632f0(int *param_1, int *param_2, int *param_3, int param_4, int param_5)
{
    unsigned int uVar4;

    *(unsigned char *)((int)param_1 + 0x92) = 1;
    uVar4 = 0;
    if (func_0206336c(param_1 + 10, param_2) != 0) {
        func_020083b0(param_1 + 10, param_2);
        uVar4 = 1;
    }
    if (func_0206336c(param_1 + 0x1f, param_3) != 0) {
        func_020083b0(param_1 + 0x1f, param_3);
        uVar4 = 1;
    }
    if (param_1[0x22] != param_4) {
        param_1[0x22] = param_4;
        uVar4 = 1;
    }
    if (param_1[0x23] != param_5) {
        param_1[0x23] = param_5;
        uVar4 = 1;
    }
    if (uVar4 != 0) {
        ((Obj020632f0 *)param_1)->m6();
    }
    return;
}
}
