void func_02073af0(unsigned int *param_1,unsigned int *param_2)
{
    unsigned int *cur;
    unsigned int *prev;

    cur = (unsigned int *)*param_1;
    while (cur != 0 && cur[28] <= param_2[28]) {
        if (cur == param_2) {
            return;
        }
        cur = (unsigned int *)cur[32];
    }
    if (cur == 0) {
        prev = (unsigned int *)param_1[1];
        if (prev == 0) {
            *param_1 = (unsigned int)param_2;
        }
        else {
            prev[32] = (unsigned int)param_2;
        }
        param_2[31] = (unsigned int)prev;
        param_2[32] = 0;
        param_1[1] = (unsigned int)param_2;
        return;
    }
    prev = (unsigned int *)cur[31];
    if (prev == 0) {
        *param_1 = (unsigned int)param_2;
    }
    else {
        prev[32] = (unsigned int)param_2;
    }
    param_2[31] = (unsigned int)prev;
    param_2[32] = (unsigned int)cur;
    cur[31] = (unsigned int)param_2;
}
