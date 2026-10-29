extern char data_0209c124;
extern char data_0209c12c;
extern char data_0209c138;
extern char data_0209c144;
extern char data_0209c14c;
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_0200b9f4();
extern int func_020121e4();

void func_02050d40(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned char auStack_130 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0200b9f4(auStack_50,param_1,((unsigned int)&data_0209c124),param_1 + 4,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_0200b9f4(auStack_88,param_1,((unsigned int)&data_0209c12c),param_1 + 0x14,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_02008a64(auStack_c0,param_1,((unsigned int)&data_0209c138),param_1 + 0x50,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_02008a64(auStack_f8,param_1,((unsigned int)&data_0209c144),param_1 + 0x54,1,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    func_020121e4(auStack_130,param_1,((unsigned int)&data_0209c14c),param_1 + 0x58,1,0);
    func_02008a00(param_2,auStack_130);
    func_020089fc(auStack_130);
    return;
}
