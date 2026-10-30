extern int func_020509b8();

int func_02051140(int *param_1,int param_2)

{
    int *piVar1;

    piVar1 = (int *)param_1[0xf];
    while (piVar1 != (int *)0x0) {
        if (func_020509b8(piVar1,param_2) != 0) {
            return 1;
        }
        piVar1 = (int *)piVar1[0x15];
    }
    return 0;
}
