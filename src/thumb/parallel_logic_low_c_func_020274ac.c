extern int func_020274e8();
extern int data_020c9fc0[];

int func_020274ac(unsigned int param_1)
{
    int iVar1;

    if ((param_1 & 0x1000) != 0) {
        if ((0xffffefff & param_1) >= 0x400) {
            return data_020c9fc0[1];
        }
        return data_020c9fc0[0];
    }
    if ((param_1 & 0x8000) != 0) {
        return data_020c9fc0[3];
    }
    iVar1 = func_020274e8();
    return iVar1;
}
