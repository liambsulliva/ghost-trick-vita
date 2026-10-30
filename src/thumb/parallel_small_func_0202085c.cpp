class T0202085c {
public:
    virtual int method(int arg);
};

extern "C" {
extern int func_0202037c(...);
extern int func_0202038c(...);

unsigned int func_0202085c(unsigned char *param_1)
{
    unsigned int uVar2;
    int iVar1;
    unsigned int (T0202085c::*pm1)(int);
    unsigned int (T0202085c::*pm2)();

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        iVar1 = func_0202038c(param_1);
        if (iVar1 != 0) {
            pm1 = *(unsigned int (T0202085c::**)(int))(param_1 + 0xc);
            uVar2 = (((T0202085c *)*(int *)(param_1 + 8))->*pm1)(*(int *)(param_1 + 0x2c));
        }
        else {
            pm2 = *(unsigned int (T0202085c::**)())(param_1 + 0xc);
            uVar2 = (((T0202085c *)*(int *)(param_1 + 8))->*pm2)();
        }
    }
    else {
        uVar2 = (unsigned int)*(unsigned int *)(*(int *)(param_1 + 0xc) + (*(int *)(param_1 + 0x2c) << 2));
    }
    return uVar2;
}
}
