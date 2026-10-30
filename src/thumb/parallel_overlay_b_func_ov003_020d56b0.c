typedef int code();
typedef struct {
    unsigned int value[3];
} triple;
extern int func_02010b60();
extern int func_02010b64();
extern int func_02010b7c();

unsigned int func_ov003_020d56b0(int param_1)
{
    unsigned char bVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int *piVar4;
    int iVar5;
    int iVar6;
    triple table;
    unsigned int result;

    result = 0;
    func_02010b7c();
    uVar2 = func_02010b60();
    func_02010b7c();
    uVar3 = func_02010b64();
    piVar4 = *(int **)(param_1 + 0x2c);
    bVar1 = 0;
    if (piVar4 != (int *)0 && (iVar5 = (**(code **)(*piVar4 + 0xc4))(piVar4, uVar2, uVar3), iVar5 != 0)) {
        bVar1 = 1;
    }
    if (!bVar1) {
        table = *(triple *)0x020d5a10;
        for (iVar5 = 0; iVar5 < 3; iVar5 = iVar5 + 1) {
            iVar6 = *(int *)(param_1 + 0x178) + iVar5 - 1;
            if (iVar6 >= 0 && iVar6 < *(int *)(param_1 + 0x180)) {
                piVar4 = *(int **)(param_1 + iVar6 * 4 + 0x44);
                iVar6 = (**(code **)(*piVar4 + 0xc0))(piVar4, uVar2, uVar3);
                if (iVar6 != 0) {
                    result = table.value[iVar5];
                    break;
                }
            }
        }
    }
    return result;
}
