extern int func_0200847c();
extern int func_0202a90c();
extern int func_02061b8c();

void func_02061be4(int param_1)

{
    unsigned short uVar1;
    unsigned int uVar2;

    uVar2 = func_0200847c();
    uVar1 = func_0202a90c(uVar2,*(unsigned int *)(param_1 + 0x28));
    *(unsigned short *)(param_1 + 0x2c) = uVar1;
    uVar2 = func_0200847c();
    uVar2 = func_02061b8c(uVar2,*(unsigned short *)(param_1 + 0x2c) & 0xff);
    *(unsigned int *)(param_1 + 0x34) = uVar2;
    return;
}
