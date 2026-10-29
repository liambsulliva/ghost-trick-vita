extern char data_02083ebc;
extern char data_02083ec8;
extern char data_02083ed0;
extern void func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_020121e4();

void func_02012164(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020121e4(auStack_50,param_1,((unsigned int)&data_02083ebc),param_1 + 4,0,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a30(auStack_88,param_1,((unsigned int)&data_02083ec8),param_1 + 6,0x20,4);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_02008a30(auStack_c0,param_1,((unsigned int)&data_02083ed0),param_1 + 0xe,0x20,4);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    return;
}
