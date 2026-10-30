class Target020203a4 {
public:
    virtual void method(unsigned int value);
};

extern "C" {
extern int func_0202037c(...);
extern int func_0202038c(...);

void func_020203a4(unsigned char *param_1, unsigned int param_2)
{
    int iVar1;
    void (Target020203a4::*pm1)(unsigned int, int);
    void (Target020203a4::*pm2)(unsigned int);

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        iVar1 = func_0202038c(param_1);
        if (iVar1 != 0) {
            pm1 = *(void (Target020203a4::**)(unsigned int, int))(param_1 + 0x1c);
            (((Target020203a4 *)*(int *)(param_1 + 8))->*pm1)(param_2, *(int *)(param_1 + 0x2c));
        }
        else {
            pm2 = *(void (Target020203a4::**)(unsigned int))(param_1 + 0x1c);
            (((Target020203a4 *)*(int *)(param_1 + 8))->*pm2)(param_2);
        }
    }
    else {
        *(unsigned char *)(*(int *)(param_1 + 0x2c) + *(int *)(param_1 + 0xc)) = (unsigned char)param_2;
    }
}
}
