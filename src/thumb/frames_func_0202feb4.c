extern char data_020856e8;
extern char data_020856f0;
extern void func_020089fc();
extern int func_02008a00();
extern int func_0200b9f4();
extern int func_02012f14();
extern int func_0202f6a0();

void func_0202feb4(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202f6a0();
    func_02012f14(auStack_50,param_1,((unsigned int)&data_020856e8),param_1 + 0xb0,0,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_0200b9f4(auStack_88,param_1,((unsigned int)&data_020856f0),param_1 + 0x28,0,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    return;
}
