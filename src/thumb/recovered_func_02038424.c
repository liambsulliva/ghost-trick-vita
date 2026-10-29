extern int func_02037130();

unsigned int func_02038424(int param_1)

{
    int iVar1;

    if ((*(char *)(param_1 + 0x32) == '\0') &&
          (iVar1 = func_02037130(*(unsigned int *)(param_1 + 0x1c) & 0xff), *(short *)(param_1 + 0x24) == iVar1))
    {
        return 1;
    }
    return 0;
}
