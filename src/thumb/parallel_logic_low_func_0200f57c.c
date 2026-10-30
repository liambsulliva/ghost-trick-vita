void func_0200f57c(char *param_1)
{
    if (*(int *)(param_1 + 0xd94) < 0) {
        *(int *)(param_1 + 0xd94) = 0;
    }
    if (*(int *)(param_1 + 0xd98) < 0) {
        *(int *)(param_1 + 0xd98) = 0;
    }
    if (*(int *)(param_1 + 0xd94) > *(int *)(param_1 + 0xdcc)) {
        *(int *)(param_1 + 0xd94) = *(int *)(param_1 + 0xdcc);
    }
    if (*(int *)(param_1 + 0xd98) > *(int *)(param_1 + 0xdd0)) {
        *(int *)(param_1 + 0xd98) = *(int *)(param_1 + 0xdd0);
    }
}
