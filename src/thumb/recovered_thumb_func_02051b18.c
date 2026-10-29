extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_020285d4();

void func_02051b18(int param_1,unsigned int param_2)

{
    unsigned char auStack_48 [56];

    func_020285d4();
    func_02008a64(auStack_48,param_1,((unsigned int)0x0209c1d0u),param_1 + 0x30,1,0);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
