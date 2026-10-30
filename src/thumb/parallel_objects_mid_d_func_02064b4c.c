extern char data_020c9e48;
extern char data_020c9e88;
extern int func_02064854();

void func_02064b4c(int param_1,int param_2,int param_3)
{
    int iVar1;

    if (param_3 == 0) {
        *(int *)(param_1 + 0x68) = param_2;
        iVar1 = func_02064854(param_2);
        *(unsigned short *)(((int)&data_020c9e48) + 0x1a) = 0x1ff & iVar1;
        return;
    }
    if (param_3 == 1) {
        *(int *)(param_1 + 0x6c) = param_2;
        iVar1 = func_02064854(param_2);
        *(unsigned short *)(((int)&data_020c9e88) + 0x26) = 0x1ff & iVar1;
    }
    return;
}
