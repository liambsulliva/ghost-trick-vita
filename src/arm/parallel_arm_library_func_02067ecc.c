void func_02067ecc(int *param_1, int param_2)
{
    unsigned short idx;
    unsigned short cnt;

    idx = *(unsigned short *)((int)param_1 + 10);
    *(int *)(param_2 + idx + 4) = 0;
    *(int *)(param_2 + idx) = 0;
    cnt = *(unsigned short *)((int)param_1 + 8);
    param_1[0] = param_2;
    param_1[1] = param_2;
    *(unsigned short *)((int)param_1 + 8) = cnt + 1;
}
