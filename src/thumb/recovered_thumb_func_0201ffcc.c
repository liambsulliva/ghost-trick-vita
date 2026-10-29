extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_0201fd8c();

void func_0201ffcc(int param_1,unsigned int param_2)

{
    unsigned char auStack_48 [56];

    func_0201fd8c();
    func_02008a64(auStack_48,param_1,((unsigned int)0x02084ea0u),param_1 + 0x10,0x21,0x2f);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
