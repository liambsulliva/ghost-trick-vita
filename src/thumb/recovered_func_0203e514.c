extern int func_02008444();
extern int func_02010384();
extern int func_0203e4a4();

void func_0203e514(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02008444();
    func_02010384(uVar1,1);
    func_0203e4a4(param_1);
    if (*(int *)(param_1 + 8) == 1) {
        *(unsigned char *)(param_1 + 0x20) = 1;
    }
    return;
}
