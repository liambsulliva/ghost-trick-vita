extern char data_02084f1c;
extern char data_02084f24;
extern char data_02084f2c;
extern char data_02084f34;
extern int func_020089c8();
extern int func_020089fc();
extern int func_02008a00();
extern int func_020121e4();

void func_02022e0c(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020121e4(auStack_50,param_1,((unsigned int)&data_02084f1c),param_1 + 0xc,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_020121e4(auStack_88,param_1,((unsigned int)&data_02084f24),param_1 + 0xd,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_020089c8(auStack_c0,param_1,((unsigned int)&data_02084f2c),param_1 + 4,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_020089c8(auStack_f8,param_1,((unsigned int)&data_02084f34),param_1 + 0x14,1,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    return;
}
