extern unsigned int func_0200b7c0(int param_1);

void func_02046884(char *param_1)
{
    int value;

    value = *(int *)(param_1 + 0xb0) - 0x400;
    *(int *)(param_1 + 0xb0) = value;
    if (value <= 0x70000) {
        *(int *)(param_1 + 0xb0) = 0x80000;
    }
    func_0200b7c0((int)(param_1 + 0x28));
}
