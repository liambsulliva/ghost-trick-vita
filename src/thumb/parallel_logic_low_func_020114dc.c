extern void func_020114d4(char *object, char *value, int size);
void func_020114dc(char *param_1, int param_2)
{
    if (param_2 != 0) {
        func_020114d4(param_1 + 0x64, param_1 + 0xe4, 0x80);
        return;
    }
    func_020114d4(param_1 + 0x64, param_1 + 0x164, 0x80);
    *(unsigned char *)(param_1 + 0x18) = (unsigned char)((*(unsigned int *)(param_1 + 0xc) << 13) >> 29);
    *(char *)(param_1 + 0x19) = (char)((*(unsigned int *)(param_1 + 0xc) << 10) >> 29);
}
