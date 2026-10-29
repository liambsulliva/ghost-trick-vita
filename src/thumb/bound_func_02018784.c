extern char data_020c9d50;
extern int func_02018e0c();
extern int func_02019358();

void func_02018784(int param_1)

{
    int iVar1;

    iVar1 = func_02018e0c(param_1,*(unsigned int *)(((int)&data_020c9d50) + 4));
    if (((iVar1 != 1) && (*(char *)(param_1 + 0x8f) != '\0')) &&
          (iVar1 = func_02019358(param_1), iVar1 == 0)) {
        *(unsigned char *)(param_1 + 0x59) = 1;
        *(unsigned char *)(param_1 + 0x8c) = 0;
    }
    return;
}
