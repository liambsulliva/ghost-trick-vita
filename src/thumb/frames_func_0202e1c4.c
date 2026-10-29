extern char data_0208554c;
extern char data_02085558;
extern void func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_02008a64();
extern int func_0202d428();

void func_0202e1c4(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202d428();
    func_02008a30(auStack_50,param_1,((unsigned int)&data_0208554c),param_1 + 0x180,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a64(auStack_88,param_1,((unsigned int)&data_02085558),param_1 + 0x188,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    return;
}
