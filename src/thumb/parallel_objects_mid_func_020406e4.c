typedef int code();
extern int func_02040e44();
extern int func_020411b8();

void func_020406e4(int param_1,unsigned short *param_2,int param_3)

{
    unsigned short sVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    *(unsigned short **)(param_1 + 0x3c) = param_2;
    if (param_2 != (unsigned short *)0x0) {
        uVar3 = (unsigned int)(unsigned short)param_2[2];
        if ((((uVar3 != 0) && (*(char *)(param_1 + 0x2c) == '\0')) &&
                (*(int **)(param_1 + 0x30) != (int *)0x0)) &&
              (uVar2 = (**(code **)(**(int **)(param_1 + 0x30) + 0x30))(*(int **)(param_1 + 0x30)), uVar3 != uVar2)) {
            func_02040e44(*(unsigned int *)(param_1 + 0x30),uVar3);
        }
        uVar3 = (unsigned int)(unsigned short)param_2[1];
        if (((uVar3 != 0) && (*(int **)(param_1 + 0x34) != (int *)0x0)) &&
              (uVar2 = (**(code **)(**(int **)(param_1 + 0x34) + 0x30))(*(int **)(param_1 + 0x34)), uVar3 != uVar2)) {
            func_02040e44(*(unsigned int *)(param_1 + 0x34),uVar3);
        }
        sVar1 = *param_2;
        if (((sVar1 != 0) && ((param_3 != 0 || (*(unsigned short *)(param_1 + 0x44) != sVar1)))) &&
              ((*(unsigned short *)(param_1 + 0x44) = sVar1, *(unsigned short *)(param_1 + 0x44) != 0 &&
                (*(int *)(param_1 + 0x38) != 0)))) {
            func_020411b8(*(int *)(param_1 + 0x38),*param_2);
        }
    }
    return;
}
