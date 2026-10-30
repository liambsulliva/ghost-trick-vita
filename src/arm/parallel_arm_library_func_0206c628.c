extern int func_0206e034();
extern int func_0206b9dc();

int func_0206c628(int param_1)
{
    int local_b;
    int local_a;

    local_b = 0;
    if (func_0206e034(param_1, &local_b) == 0)
    {
        *(int **)(param_1 + 0x10) = &local_a;
        local_a = 0;
        if (func_0206b9dc(param_1, 15, 1) != 0)
        {
            local_b = local_a;
        }
    }
    return local_b;
}
