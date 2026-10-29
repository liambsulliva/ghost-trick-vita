extern char data_0209c118;
extern char data_0209c198;
extern void func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_020285d4();

void func_020516f4(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020285d4();
    func_02008a64(auStack_50,param_1,((unsigned int)&data_0209c118),param_1 + 0x30,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a64(auStack_88,param_1,((unsigned int)&data_0209c198),param_1 + 0x34,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    return;
}
