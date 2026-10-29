extern char data_0209d99c;
extern char data_0209d9a8;
extern int func_020089c8();
extern void func_020089fc();
extern int func_02008a00();
extern int func_0200b9f4();
extern int func_02058ea0();

void func_020598b8(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_02058ea0();
    func_0200b9f4(auStack_50,param_1,((unsigned int)&data_0209d99c),param_1 + 0x114,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_020089c8(auStack_88,param_1,((unsigned int)&data_0209d9a8),param_1 + 0x144,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    return;
}
