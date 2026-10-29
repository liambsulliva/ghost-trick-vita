extern int func_0200e62c();

void func_020184f4(int param_1,unsigned short param_2,unsigned short param_3)

{
    int iVar1;

    func_0200e62c();
    iVar1 = ((int)0x00000dc4u);
    *(unsigned short *)(param_1 + ((int)0x00000dc4u)) = param_2;
    *(unsigned short *)(param_1 + iVar1 + 2) = param_3;
    return;
}
