typedef int code();
extern int func_02010b60();
extern int func_02010b64();
extern unsigned char * func_02010b7c();
extern int func_02015154();

int func_02043bb8(int param_1)

{
    unsigned char *pbVar1;
    int iVar2;
    int iVar4;

    if (*(unsigned char *)(param_1 + 0x10) == 3) {
        return 0;
    }
    pbVar1 = func_02010b7c();
    iVar2 = func_02015154(pbVar1);
    if (iVar2 != 0 || (*((unsigned int *)0x020c9d50u) & (0xf << 8)) != 0) {
        if (*(int *)(param_1 + 0x38) != 0) {
            pbVar1 = func_02010b7c();
            iVar2 = func_02010b60(pbVar1);
            pbVar1 = func_02010b7c();
            iVar4 = func_02010b64(pbVar1);
            iVar2 = (**(code **)(**(int **)(param_1 + 0x38) + 0xbc))(*(int **)(param_1 + 0x38),iVar2,iVar4);
            if (iVar2 != 0) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}
