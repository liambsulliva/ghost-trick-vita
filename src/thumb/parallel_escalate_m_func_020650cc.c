void func_020648a8(void *object, int value);
void func_02065168(char *object, int first, int second);
void func_02065170(char *object, int first, int second);
typedef void code(int);

void func_020650cc(int *param_1)
{
    int iVar1;
    unsigned int uVar2;
    int *piVar3;
    unsigned int uVar4;
    int iVar5;

    uVar2 = 0;
    for (uVar4 = 1; uVar4 < *(unsigned char *)(param_1 + 39); uVar4 = uVar4 + 1) {
        if (*(unsigned int *)((char *)param_1 + uVar2 * 4 + 0x58) < *(unsigned int *)((char *)param_1 + uVar4 * 4 + 0x58)) {
            uVar2 = uVar4;
        }
    }
    func_020648a8(*(void **)((char *)param_1 + uVar2 * 4 + 0x38), *(int *)((char *)param_1 + uVar2 * 4 + 0x58));
    uVar4 = 0;
    piVar3 = (int *)((int)param_1 + uVar2 * 4);
    do {
        iVar5 = uVar4 * 4;
        iVar1 = *(int *)((char *)param_1 + iVar5 + 0x88);
        if (iVar1 != 0) {
            func_02065168((char *)iVar1, piVar3[14], piVar3[22]);
            func_02065170(*(char **)((char *)param_1 + iVar5 + 0x88), param_1[18], 0x1000);
            iVar1 = *(int *)((char *)param_1 + iVar5 + 0x88);
            (**(code **)(*(int *)iVar1))(iVar1);
        }
        iVar1 = *(int *)((char *)param_1 + iVar5 + 0x90);
        if (iVar1 != 0) {
            func_02065168((char *)iVar1, piVar3[14], piVar3[22]);
            func_02065170(*(char **)((char *)param_1 + iVar5 + 0x90), param_1[18], 0x1000);
            iVar1 = *(int *)((char *)param_1 + iVar5 + 0x90);
            (**(code **)(*(int *)iVar1))(iVar1);
        }
        uVar4 = uVar4 + 1;
    } while (uVar4 < 2);
}
