extern int func_02023e28();
extern int func_02023e6c();

void func_02023e44(int param_1,unsigned int param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;

    uVar2 = *(unsigned int *)(param_1 + (param_2 & 0x3ff) * 4 + 0x20);
    uVar1 = func_02023e28(uVar2);
    if ((((unsigned int)0xfffffc00u) & param_2) == uVar1) {
        func_02023e6c(uVar2);
    }
    return;
}
