void func_02062c0c(int param_1, int param_2)
{
    unsigned int i;
    int value;

    for (i = 0; i < 0x10; i++) {
        value = *(int *)(param_1 + i * 4 + 0xb0);
        if (value == param_2) {
            *(int *)(param_1 + i * 4 + 0xb0) = 0;
            return;
        }
    }
}
