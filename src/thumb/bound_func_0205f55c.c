extern char data_020cff60;
extern int func_0206f28c();

int func_0205f55c(int param_1,int param_2)

{
    int iVar1;
    int iVar2;
    unsigned int uVar3;

    iVar2 = param_1;
    for (uVar3 = 0; uVar3 < *(unsigned char *)(((int)&data_020cff60) + 1) + 1; uVar3 = uVar3 + 1) {
        if (param_2 == 0) {
            iVar1 = func_0206f28c(0x1000 - iVar2,param_1);
            iVar2 = iVar2 + iVar1;
        }
        else {
            iVar2 = func_0206f28c(iVar2,param_1);
        }
    }
    return iVar2;
}
