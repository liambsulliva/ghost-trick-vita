extern int func_020689a8();

void func_02068c84(unsigned int *param_1, unsigned int *param_2)
{
    unsigned int pair[2];
    unsigned int *node;

    node = (unsigned int *)*param_1;
    while (node != 0)
    {
        pair[0] = *node;
        pair[1] = *node + node[1];
        if (func_020689a8(param_1, param_2, pair) == 0)
        {
            node = (unsigned int *)node[3];
        }
        else
        {
            node[0] = pair[0];
            node[1] = pair[1] - pair[0];
            node = (unsigned int *)*param_1;
        }
    }
}
