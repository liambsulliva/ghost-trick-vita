unsigned int func_ov006_020d42b8(int param_1)

{
    if (*(char *)(param_1 + 0x28) != '\0') {
        return 0;
    }
    if (*(int *)(param_1 + 0x94) == 0) {
        return 0;
    }
    return 1;
}
