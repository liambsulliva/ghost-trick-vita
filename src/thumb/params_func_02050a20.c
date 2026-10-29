extern int func_02050a34();

void func_02050a20(unsigned char *param_1)

{
    *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_1 + 0x34);
    func_02050a34(param_1);
    *(unsigned int *)(param_1 + 0xc) = 0;
    return;
}
