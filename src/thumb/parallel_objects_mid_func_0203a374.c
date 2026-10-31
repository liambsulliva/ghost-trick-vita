extern void func_0203a338();

void func_0203a374(unsigned char *param_1)
{
    if (param_1[0x10] == 0) {
        func_0203a338(param_1);
        *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) - 0x180000;
        param_1[0x10] = 1;
        return;
    }
    if (param_1[0x10] == 1) {
        return;
    }
    if (param_1[0x10] == 2) {
        if (*(int *)(param_1 + 0x28) < *(int *)(param_1 + 0xc8)) {
            *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + 0xc000;
        }
        else {
            *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0xc8);
            param_1[0x10] = 5;
        }
    }
    if (param_1[0x10] == 3) {
        if (*(int *)(param_1 + 0x28) < *(int *)(param_1 + 0xc8)) {
            *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + 0xc000;
        }
        else {
            *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0xc8);
            param_1[0x10] = 5;
        }
    }
    if (param_1[0x10] == 4) {
        int iVar3;
        int iVar4;
        iVar3 = *(int *)(param_1 + 0xc8);
        iVar4 = *(int *)(param_1 + 0x28);
        if (iVar4 > iVar3 - 0x180000) {
            *(int *)(param_1 + 0x28) = iVar4 - 0x12000;
        }
        else {
            *(int *)(param_1 + 0x28) = iVar3 + 0x180000;
        }
    }
}
