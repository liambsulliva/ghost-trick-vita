extern int data_02083c80[];

unsigned int func_0200e26c(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    uVar2 = 0;
    if (*(int *)(param_1 + 4) < 3) {
        iVar1 = data_02083c80[(*(int *)(param_1 + 4)) * 4];
        if (iVar1 == 1) {
            uVar2 = 0xc0;
        }
        else if (iVar1 == 0) {
            uVar2 = 0x80;
        }
    }
    return uVar2;
}
