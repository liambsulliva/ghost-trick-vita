extern char data_0209d870;
extern char data_0209d87c;
extern char data_0209d884;
extern void func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_0202d650();
extern int func_02030024();

void func_02056d98(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_02030024();
    func_02008a30(auStack_50,param_1,((unsigned int)&data_0209d870),param_1 + 0x42,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_0202d650(auStack_88,param_1,((unsigned int)&data_0209d87c),param_1 + 0x44,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_0202d650(auStack_c0,param_1,((unsigned int)&data_0209d884),param_1 + 0x45,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    return;
}
