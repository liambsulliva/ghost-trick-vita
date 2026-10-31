extern int func_0207b694();
extern void func_020083b0();
extern void func_02027a5c();

void func_02059f3c(int param_1, int *param_2, int param_3, int param_4, int param_5)
{
    int iVar1;
    int iVar2;

    if (param_3 == 0) {
        param_3 = 1;
    }
    if (param_4 == 0x10) {
        func_02027a5c(param_1, param_5);
    }
    switch (param_4) {
    default:
        if (param_4 == 0x10) {
            goto code_r0x0205a03a;
        }
        return;
    case 1:
        iVar1 = *param_2;
        *(int *)(param_1 + 0x118) = iVar1;
        *(int *)(param_1 + 0x120) = param_3;
        iVar2 = func_0207b694(iVar1 - *(int *)(param_1 + 0x80), param_3);
        *(int *)(param_1 + 0x11c) = iVar2;
        *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) | 1;
        return;
    case 2:
        func_020083b0(param_1 + 0xfc, param_2);
        *(int *)(param_1 + 0x114) = param_3;
        iVar2 = func_0207b694(*(int *)(param_1 + 0xfc) - *(int *)(param_1 + 0x28), param_3);
        *(int *)(param_1 + 0x108) = iVar2;
        iVar2 = func_0207b694(*(int *)(param_1 + 0x100) - *(int *)(param_1 + 0x2c), param_3);
        *(int *)(param_1 + 0x10c) = iVar2;
        *(int *)(param_1 + 0x110) = 0;
        *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) | 2;
        return;
    case 4:
        iVar1 = *param_2;
        *(int *)(param_1 + 0x124) = iVar1;
        *(int *)(param_1 + 0x12c) = param_3;
        iVar2 = func_0207b694(iVar1 - *(int *)(param_1 + 0x84), param_3);
        *(int *)(param_1 + 0x128) = iVar2;
        *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) | 4;
        return;
    case 8:
        iVar1 = *param_2;
        *(int *)(param_1 + 0x130) = iVar1;
        if (iVar1 > 0xff) {
            *(int *)(param_1 + 0x130) = iVar1 >> 12;
        }
        *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) | 8;
        return;
    case 0:
    case 3:
    case 5:
    case 6:
    case 7:
        goto code_r0x0205a056;
    }
code_r0x0205a03a:
    iVar1 = *param_2;
    *(int *)(param_1 + 0x134) = iVar1;
    if (iVar1 >= 0x1000) {
        *(int *)(param_1 + 0x134) = iVar1 >> 12;
    }
    *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) | 0x10;
code_r0x0205a056:
    return;
}
