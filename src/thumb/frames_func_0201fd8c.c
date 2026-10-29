extern char data_02084e8c;
extern char data_02084e98;
extern int func_020089c8();
extern void func_020089fc();
extern int func_02008a00();

void func_0201fd8c(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020089c8(auStack_50,param_1,((unsigned int)&data_02084e8c),param_1 + 4,0,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_020089c8(auStack_88,param_1,((unsigned int)&data_02084e98),param_1 + 8,0,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    return;
}
