extern int func_02014b44();

void func_02015288(int param_1)

{
    func_02014b44(param_1 + ((int)0x00000f04u));
    *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_1 + 8) & 0xfffffffb;
    return;
}
