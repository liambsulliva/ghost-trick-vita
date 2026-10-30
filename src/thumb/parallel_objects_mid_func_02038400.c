int func_02038400(int *param_1, int param_2)
{
    if (param_2 > 0) {
        if ((unsigned int)(param_1[7] + param_2) >= (unsigned int)param_1[6]) {
            return 1;
        }
    }
    else if (param_2 < 0) {
        if (param_1[7] + param_2 < 0) {
            return 1;
        }
    }
    return 0;
}
