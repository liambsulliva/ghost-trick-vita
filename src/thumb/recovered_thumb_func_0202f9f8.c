extern int func_020089fc();
extern int func_02008a00();
extern int func_0200b9f4();

void func_0202f9f8(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_48 [56];
    unsigned int uStack_10;

    uStack_10 = param_4;
    func_0200b9f4(auStack_48,param_1,((unsigned int)0x020856a0u),param_1 + 0x24,0x20,0x10);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
