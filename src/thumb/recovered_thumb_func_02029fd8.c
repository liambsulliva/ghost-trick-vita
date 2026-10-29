extern int func_020089c8();
extern int func_020089fc();
extern int func_02008a00();

void func_02029fd8(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_48 [56];
    unsigned int uStack_10;

    uStack_10 = param_4;
    func_020089c8(auStack_48,param_1,((unsigned int)0x02085268u),param_1 + 0x34,1,0);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
