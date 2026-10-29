extern int func_02009900();
extern int func_0202fad4();

void func_02037d6c(int param_1)

{
    unsigned int uVar1;

    if (*(int *)(param_1 + 0x50) == 2) {
        uVar1 = func_02009900();
        func_0202fad4(uVar1,((unsigned int)0x020ced8cu));
        *(unsigned int *)(param_1 + 0x50) = 1;
    }
    return;
}
