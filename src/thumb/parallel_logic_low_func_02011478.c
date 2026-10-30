extern int func_020114d4();

void func_02011478(int param_1, int param_2)
{
    unsigned int uVar1;

    if (param_2 != 0) {
        func_020114d4(param_1 + 0xe4, param_1 + 0x64, 0x80);
        func_020114d4(param_1 + 0xe4, param_1 + 0x164, 0x80);
        return;
    }
    func_020114d4(param_1 + 0x164, param_1 + 0x64, 0x80);
    uVar1 = *(unsigned int *)(param_1 + 0xc);
    uVar1 = (uVar1 & 0xfff8ffff) | ((((unsigned int)*(unsigned char *)(param_1 + 0x18)) << 29) >> 13);
    uVar1 = (uVar1 & 0xffc7ffff) | ((((unsigned int)*(unsigned char *)(param_1 + 0x19)) << 29) >> 10);
    *(unsigned int *)(param_1 + 0xc) = uVar1;
}
