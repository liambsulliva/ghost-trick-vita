class T02020930 {
public:
    virtual int method(int arg);
};

extern "C" {
extern int func_0202037c(...);
extern int func_0202038c(...);

int func_02020930(unsigned char *param_1)
{
    int uVar2;
    int iVar1;
    int (T02020930::*pm1)(int);
    int (T02020930::*pm2)();

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        iVar1 = func_0202038c(param_1);
        if (iVar1 != 0) {
            pm1 = *(int (T02020930::**)(int))(param_1 + 0xc);
            uVar2 = (((T02020930 *)*(int *)(param_1 + 8))->*pm1)(*(int *)(param_1 + 0x2c));
        }
        else {
            pm2 = *(int (T02020930::**)())(param_1 + 0xc);
            uVar2 = (((T02020930 *)*(int *)(param_1 + 8))->*pm2)();
        }
    }
    else {
        uVar2 = (int)*(signed char *)(*(int *)(param_1 + 0x2c) + *(int *)(param_1 + 0xc));
    }
    return uVar2;
}
}
