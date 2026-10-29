extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_020629fc();

void func_02062ce0(int param_1,unsigned int param_2)

{
    unsigned char auStack_48 [56];

    func_020629fc();
    func_02008a64(auStack_48,param_1,((unsigned int)0x0209dec0u),param_1 + 0xf4,1,0);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
