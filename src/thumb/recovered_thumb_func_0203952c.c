extern int func_020394e4();
extern int func_02039558();

void func_0203952c(int param_1)

{
    *(unsigned char *)(param_1 + 0x10) = 0x38;
    *(unsigned char *)(param_1 + 0xc1) = 0x14;
    func_02039558(param_1 + 0x28,0x1000,0x1000,0);
    func_020394e4(param_1 + 0x28,3);
    return;
}
