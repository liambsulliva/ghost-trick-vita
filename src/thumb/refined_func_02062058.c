extern char data_0209de44;
extern char data_0209de54;
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_020121e4();
extern int func_02056e1c();

void func_02062058(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_02056e1c();
    func_020121e4(auStack_50,param_1,((unsigned int)&data_0209de44),param_1 + 0x7c,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a30(auStack_88,param_1,((unsigned int)&data_0209de54),param_1 + 0x7e,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    return;
}
