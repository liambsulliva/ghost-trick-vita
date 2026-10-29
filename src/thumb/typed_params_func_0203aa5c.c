extern int func_02037aa4();
extern int func_0203ae58();

void func_0203aa5c(int param_1,int param_2)

{
    int iVar1;


    if (((*(char *)(param_1 + 0x10) == '\x06') && (*(char *)(param_1 + 0xe5) == '\0')) &&
          (iVar1 = func_0203ae58((int *)param_1,param_2), iVar1 != 0)) {
        func_02037aa4(0xb);
    }
    return;
}
