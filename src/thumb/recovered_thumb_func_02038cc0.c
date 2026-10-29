extern int func_0200b518();
extern int func_0200b598();
extern int func_020280a8();

void func_02038cc0(int param_1)

{
    *(unsigned char *)(param_1 + 0x10) = 0;
    func_0200b518(param_1 + 0x28,0,0x80000000,0x80000000);
    func_0200b598(param_1 + 0x28,*(unsigned int *)(param_1 + 0xbc),0xf);
    func_020280a8(param_1 + 0x28,*(unsigned char *)(param_1 + 0xc0));
    *(unsigned int *)(param_1 + 0xc4) = *(unsigned int *)(param_1 + 0xb0);
    *(unsigned int *)(param_1 + 200) = *(unsigned int *)(param_1 + 0xb4);
    return;
}
