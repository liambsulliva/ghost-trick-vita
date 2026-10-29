extern int func_0200bab8();
extern int func_0200bbbc();

void func_02008a98(int param_1)

{
    func_0200bab8(*(int *)(param_1 + 0x14) + 0x28,*(unsigned int *)(param_1 + 0x24));
    *(unsigned int *)(param_1 + 0x24) = 0xffffffff;
    *(unsigned int *)(param_1 + 0x20) = 0;
    func_0200bbbc(*(int *)(param_1 + 0x14) + 0x28);
    return;
}
