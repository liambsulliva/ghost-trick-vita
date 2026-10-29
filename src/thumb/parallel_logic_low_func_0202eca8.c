extern int func_0202d68c();

int func_0202eca8(int param_1)

{
    char cVar1;
    int iVar2;

    if (*(char *)(param_1 + 0xc) == '\0') {
        return *(int *)(param_1 + 4);
    }
    cVar1 = *(char *)(param_1 + 1);
    if (cVar1 == '\b') {
        return *(int *)(*(int *)(param_1 + 8) + 0x40);
    }
    if (cVar1 == '\t') {
        return *(int *)(*(int *)(param_1 + 8) + 0x44);
    }
    if (cVar1 == '\n') {
        return *(int *)(*(int *)(param_1 + 8) + 0x1c);
    }
    if (cVar1 == '\v') {
        return *(int *)(param_1 + 4);
    }
    if (cVar1 == '\f') {
        return *(int *)(param_1 + 4);
    }
    if (*(int *)(param_1 + 8) != 0) {
        return func_0202d68c(*(int *)(param_1 + 8),*(int *)(param_1 + 4));
    }
    return *(int *)(param_1 + 4);
}
