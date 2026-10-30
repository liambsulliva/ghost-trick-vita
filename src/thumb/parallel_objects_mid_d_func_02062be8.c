void func_02062be8(int param_1, int param_2)
{
    unsigned int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(param_1 + i * 4 + 0xb0) == 0) {
            *(int *)(param_1 + i * 4 + 0xb0) = param_2;
            return;
        }
    }
}
