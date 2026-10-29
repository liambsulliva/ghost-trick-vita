extern char data_0209c12c;
extern char data_0209c1d8;
extern char data_0209c1e4;
extern char data_0209c1ec;
extern int func_020089c8();
extern void func_020089fc();
extern int func_02008a00();
extern int func_0200b374();
extern int func_0200b9f4();
extern int func_0202f6a0();

void func_02051b54(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202f6a0();
    func_0200b9f4(auStack_50,param_1,((unsigned int)&data_0209c12c),param_1 + 0x28,0,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_020089c8(auStack_88,param_1,((unsigned int)&data_0209c1d8),param_1 + 0x5c,0x20,8);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_0200b374(auStack_c0,param_1,((unsigned int)&data_0209c1e4),param_1 + 0x7c,0,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_0200b374(auStack_f8,param_1,((unsigned int)&data_0209c1ec),param_1 + 0x80,0,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    return;
}
