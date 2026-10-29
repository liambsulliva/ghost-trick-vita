extern char data_0209bb68;
extern char data_0209bb74;
extern char data_0209bb84;
extern char data_0209bb98;
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_020471b0();

void func_0204caf4(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020471b0();
    func_02008a64(auStack_50,param_1,((unsigned int)&data_0209bb68),param_1 + 0xb8,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a64(auStack_88,param_1,((unsigned int)&data_0209bb74),param_1 + 0xbc,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_02008a64(auStack_c0,param_1,((unsigned int)&data_0209bb84),param_1 + 0xc0,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_02008a64(auStack_f8,param_1,((unsigned int)&data_0209bb98),param_1 + 0xc4,1,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    return;
}
