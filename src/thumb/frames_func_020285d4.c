extern char data_0208511c;
extern char data_02085120;
extern char data_0208512c;
extern char data_02085134;
extern char data_0208513c;
extern char data_02085148;
extern char data_02085150;
extern char data_02085158;
extern int func_020089c8();
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_0200b374();
extern int func_020121e4();
extern int func_02012ea8();

void func_020285d4(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned char auStack_130 [56];
    unsigned char auStack_168 [56];
    unsigned char auStack_1a0 [56];
    unsigned char auStack_1d8 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0200b374(auStack_50,param_1,((unsigned int)&data_0208511c),param_1 + 8,0,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_0200b374(auStack_88,param_1,((unsigned int)&data_02085120),param_1 + 0xc,0,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_020121e4(auStack_c0,param_1,((unsigned int)&data_0208512c),param_1 + 0x10,0,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_02012ea8(auStack_f8,param_1,((unsigned int)&data_02085134),param_1 + 0x11,0,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    func_02008a64(auStack_130,param_1,((unsigned int)&data_0208513c),param_1 + 0x14,0,0);
    func_02008a00(param_2,auStack_130);
    func_020089fc(auStack_130);
    func_0200b374(auStack_168,param_1,((unsigned int)&data_02085148),param_1 + 0x18,0,0);
    func_02008a00(param_2,auStack_168);
    func_020089fc(auStack_168);
    func_020089c8(auStack_1a0,param_1,((unsigned int)&data_02085150),param_1 + 0x1c,0,0);
    func_02008a00(param_2,auStack_1a0);
    func_020089fc(auStack_1a0);
    func_020089c8(auStack_1d8,param_1,((unsigned int)&data_02085158),param_1 + 0x20,0x20,
                                *(unsigned int *)(param_1 + 0x1c));
    func_02008a00(param_2,auStack_1d8);
    func_020089fc(auStack_1d8);
    return;
}
