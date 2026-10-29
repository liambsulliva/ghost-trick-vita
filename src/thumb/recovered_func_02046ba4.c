unsigned int func_02046ba4(int param_1)

{
    char cVar1;

    cVar1 = *(char *)(*(int *)(param_1 + 0x28) + 0x44);
    if ((cVar1 == '\x01') || (cVar1 == '\x03')) {
        return 1;
    }
    return 0;
}
