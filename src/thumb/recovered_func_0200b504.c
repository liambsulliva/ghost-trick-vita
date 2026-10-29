extern int func_0200b208();
extern int func_02023f24();

void func_0200b504(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_0200b208();
    func_02023f24(uVar1,*(unsigned int *)(param_1 + 4));
    *(unsigned int *)(param_1 + 4) = 0;
    return;
}
