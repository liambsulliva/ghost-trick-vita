extern int func_02009a0c();
extern int func_0201e35c();

void func_0200a478(int param_1)

{
    int iVar1;

    func_02009a0c((unsigned char *)(param_1 + ((int)0x00007a04u)),0x3000);
    iVar1 = ((int)0x0000aa04u);
    func_0201e35c(param_1 + ((int)0x0000aa04u));
    *(unsigned short *)(param_1 + iVar1 + 0x28) = 0;
    return;
}
