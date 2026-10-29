extern unsigned char * func_02010b7c();
extern int func_02015154();
extern int func_02010b64();

int func_020357a4(int param_1)

{
    unsigned char bVar1;
    unsigned char *pbVar2;
    int iVar3;
    short sVar4;

    pbVar2 = func_02010b7c();
    if (func_02015154(pbVar2) != 0) {
        pbVar2 = func_02010b7c();
        sVar4 = (short)func_02010b64(pbVar2);
        bVar1 = *(unsigned char *)(param_1 + 0x3c);
        iVar3 = 0;
        if (bVar1 < sVar4 && sVar4 < *(unsigned char *)(param_1 + 0x3d)) {
            iVar3 = 1;
        }
    }
    else {
        iVar3 = 0;
    }
    return iVar3;
}
