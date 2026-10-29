extern int func_0203166c();

void func_0203164c(int param_1)

{
    int iVar1;

    iVar1 = ((int)0x0000103eu);
    if (*(char *)(param_1 + ((int)0x0000103eu)) != '\0') {
        func_0203166c(*(unsigned int *)(param_1 + ((int)0x0000103eu) + 2));
        *(unsigned char *)(param_1 + iVar1) = 0;
    }
    return;
}
