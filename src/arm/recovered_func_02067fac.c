unsigned int func_02067fac(unsigned int *param_1,int param_2)

{
    unsigned int uVar1;

    if (param_2 == 0) {
        uVar1 = *param_1;
    }
    else {
        uVar1 = *(unsigned int *)(param_2 + (unsigned int)*(unsigned short *)((int)param_1 + 10) + 4);
    }
    return uVar1;
}
