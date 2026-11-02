struct func_02010874_obj
{
    char pad0[8];
    unsigned int low : 10;
    unsigned int bit10 : 1;
    unsigned int high : 21;
};

void func_020109c8(int a, int b, int c);

void func_02010874(int param_1)
{
    unsigned int saved;
    unsigned int i;
    int base;
    int offset;

    base = 0xe1c;
    *(int *)(param_1 + base) = 0;
    i = 2;
    do {
        offset = i * 4;
        i = i + 1;
        *(int *)(param_1 + offset + base) = 0;
    } while (i < 8);
    saved = ((struct func_02010874_obj *)param_1)->bit10;
    ((struct func_02010874_obj *)param_1)->bit10 = 1;
    func_020109c8(param_1, 1, 1);
    ((struct func_02010874_obj *)param_1)->bit10 = saved;
}
