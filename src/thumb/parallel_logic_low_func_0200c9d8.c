extern int func_0207dc50();
extern void func_02022514();
extern void func_0200ca14();
extern void func_0200ca64();

unsigned int *func_0200c9d8(unsigned int *param_1, unsigned int param_2, int param_3, unsigned int param_4, unsigned int param_5)
{
    int iVar1;

    *param_1 = 0x020a2dccu;
    func_02022514((unsigned char *)(param_1 + 1), param_3, param_4, param_5);
    param_1[0xe] = param_2;
    iVar1 = func_0207dc50(param_1[0xe], 0x18, 8, (unsigned char *)((unsigned int)func_0200ca14), (unsigned int)func_0200ca64);
    param_1[0xf] = iVar1;
    return param_1;
}
