extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_02065e40();

void func_02066b44(int param_1,unsigned int param_2)

{
    unsigned char auStack_48 [56];

    func_02065e40();
    func_02008a30(auStack_48,param_1,((unsigned int)0x0209e1c0u),param_1 + 0xd6,1,0);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
