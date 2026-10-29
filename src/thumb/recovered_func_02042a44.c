void func_02042a44(int param_1)

{
    if ((*(int *)(param_1 + 4) != -1) && (*(int *)(param_1 + 0xc) == -1)) {
        *(unsigned int *)(param_1 + 4) = 0xffffffff;
        *(unsigned char *)(param_1 + 0x38) = 0;
        *(unsigned char *)(param_1 + 0x3a) = 1;
    }
    return;
}
