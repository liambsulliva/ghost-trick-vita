extern void func_020079b4(int a,int b);

void func_02003b1c(int param_1)
{
    int *base;
    int elem;

    base = (int *)0x020b18a0;
    elem = base[4] + 0x15bc + param_1 * 0x4c;
    func_020079b4(param_1,1);
    *(int *)(elem + 0x48) = 1;
    *(unsigned int *)(elem + 0x3c) = *(unsigned int *)(elem + 0x3c) | 4;
}
