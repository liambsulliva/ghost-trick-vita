class Target020206a4 {
public:
    virtual void method(unsigned long long value);
};

extern "C" {
extern int func_0202037c(...);
extern int func_0202038c(...);

void func_020206a4(unsigned char *param_1, unsigned long long param_2)
{
    int iVar1;
    void (Target020206a4::*pm1)(unsigned long long, int);
    void (Target020206a4::*pm2)(unsigned long long);

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        iVar1 = func_0202038c(param_1);
        if (iVar1 != 0) {
            pm1 = *(void (Target020206a4::**)(unsigned long long, int))(param_1 + 0x1c);
            (((Target020206a4 *)*(int *)(param_1 + 8))->*pm1)(param_2, *(int *)(param_1 + 0x2c));
        }
        else {
            pm2 = *(void (Target020206a4::**)(unsigned long long))(param_1 + 0x1c);
            (((Target020206a4 *)*(int *)(param_1 + 8))->*pm2)(param_2);
        }
    }
    else {
        ((unsigned long long *)*(int *)(param_1 + 0xc))[*(int *)(param_1 + 0x2c)] = param_2;
    }
}
}
