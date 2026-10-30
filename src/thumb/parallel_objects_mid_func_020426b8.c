void func_020426b8(char *param_1)
{
    *(char **)(*(char **)(param_1 + 0x1c) + 0x20) = *(char **)(param_1 + 0x20);
    if (*(char **)(param_1 + 0x20) != 0) {
        *(char **)(*(char **)(param_1 + 0x20) + 0x1c) = *(char **)(param_1 + 0x1c);
    }
}
