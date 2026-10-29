extern char data_02085704;
extern char data_0208570c;
extern char data_02085714;
extern void func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_02012f14();
extern int func_0202f6a0();

void func_02030024(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202f6a0();
    func_02012f14(auStack_50,param_1,((unsigned int)&data_02085704),param_1 + 0x28,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a30(auStack_88,param_1,((unsigned int)&data_0208570c),param_1 + 0x40,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_02012f14(auStack_c0,param_1,((unsigned int)&data_02085714),param_1 + 0x34,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    return;
}
