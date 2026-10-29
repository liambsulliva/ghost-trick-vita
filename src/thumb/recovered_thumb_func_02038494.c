extern int func_0201fbec();
extern int func_0201fcd0();
extern int func_02037130();

void func_02038494(int param_1)

{
    int iVar1;

    if ((*(char *)(param_1 + 0x32) == '\0') &&
          (iVar1 = func_02037130(*(unsigned int *)(param_1 + 0x1c) & 0xff), iVar1 != -1)) {
        func_0201fbec(*(unsigned int *)(param_1 + 0x14),iVar1);
        func_0201fcd0(*(unsigned int *)(param_1 + 0x14),iVar1);
    }
    return;
}
