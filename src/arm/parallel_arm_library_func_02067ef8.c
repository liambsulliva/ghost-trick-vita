extern int func_02067ecc();

void func_02067ef8(int *param_1,int param_2)

{
    unsigned short uVar1;

    if (*param_1 == 0) {
        func_02067ecc(param_1,param_2);
        return;
    }
    uVar1 = *(unsigned short *)((int)param_1 + 10);
    *(int *)(param_2 + (unsigned int)uVar1) = param_1[1];
    *(unsigned int *)(param_2 + (unsigned int)uVar1 + 4) = 0;
    *(int *)(param_1[1] + (unsigned int)*(unsigned short *)((int)param_1 + 10) + 4) = param_2;
    param_1[1] = param_2;
    *(unsigned short *)(param_1 + 2) = *(unsigned short *)(param_1 + 2) + 1;
    return;
}
