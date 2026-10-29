extern int func_0202d700();

void func_0202ecf8(int param_1,unsigned int param_2)

{
    char cVar1;

    if (*(char *)(param_1 + 0xc) == '\0') {
        *(unsigned int *)(param_1 + 4) = param_2;
        return;
    }
    cVar1 = *(char *)(param_1 + 1);
    if (cVar1 == '\b') {
        *(unsigned int *)(*(int *)(param_1 + 8) + 0x40) = param_2;
        return;
    }
    if (cVar1 == '\t') {
        *(unsigned int *)(*(int *)(param_1 + 8) + 0x44) = param_2;
        return;
    }
    if (cVar1 == '\n') {
        *(unsigned int *)(*(int *)(param_1 + 8) + 0x1c) = param_2;
        return;
    }
    if ((cVar1 != '\v') && (cVar1 != '\f')) {
        func_0202d700(*(int *)(param_1 + 8),*(int *)(param_1 + 4),param_2);
    }
    return;
}
