class Target020208c4 {
public:
    virtual int method(int arg);
};

extern "C" {
extern int func_0202037c(...);
extern int func_0202038c(...);

unsigned long long func_020208c4(unsigned char *param_1)
{
    unsigned long long uVar2;
    int iVar1;
    unsigned long long (Target020208c4::*pm1)(int);
    unsigned long long (Target020208c4::*pm2)();

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        iVar1 = func_0202038c(param_1);
        if (iVar1 != 0) {
            pm1 = *(unsigned long long (Target020208c4::**)(int))(param_1 + 0xc);
            uVar2 = (((Target020208c4 *)*(int *)(param_1 + 8))->*pm1)(*(int *)(param_1 + 0x2c));
        }
        else {
            pm2 = *(unsigned long long (Target020208c4::**)())(param_1 + 0xc);
            uVar2 = (((Target020208c4 *)*(int *)(param_1 + 8))->*pm2)();
        }
    }
    else {
        uVar2 = ((unsigned long long *)*(int *)(param_1 + 0xc))[*(int *)(param_1 + 0x2c)];
    }
    return uVar2;
}
}
