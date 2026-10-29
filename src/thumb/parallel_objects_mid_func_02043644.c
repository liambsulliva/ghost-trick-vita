extern int func_02043400();

unsigned int func_02043644(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int *piVar1;

    piVar1 = (int *)func_02043400(param_1,3);
    if (piVar1 != (int *)0x0) {
        if (*piVar1 != 0) {
            return 1;
        }
        return 0;
    }
    return 0;
}
