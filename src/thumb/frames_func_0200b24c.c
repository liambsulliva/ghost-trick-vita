extern char data_02083b28;
extern char data_02083b30;
extern char data_02083b38;
extern char data_02083b40;
extern char data_02083b48;
extern char data_02083b50;
extern int func_020089c8();
extern void func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_0200b340();
extern int func_0200b374();

void func_0200b24c(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned char auStack_130 [56];
    unsigned char auStack_168 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_02008a30(auStack_50,param_1,((unsigned int)&data_02083b28),param_1 + 0x14,0,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_0200b340(auStack_88,param_1,((unsigned int)&data_02083b30),param_1 + 0x16,0,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_0200b340(auStack_c0,param_1,((unsigned int)&data_02083b38),param_1 + 0x18,0,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_020089c8(auStack_f8,param_1,((unsigned int)&data_02083b40),param_1 + 0x1c,0,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    func_0200b374(auStack_130,param_1,((unsigned int)&data_02083b48),param_1 + 0x20,0,0);
    func_02008a00(param_2,auStack_130);
    func_020089fc(auStack_130);
    func_0200b374(auStack_168,param_1,((unsigned int)&data_02083b50),param_1 + 0x24,0,0);
    func_02008a00(param_2,auStack_168);
    func_020089fc(auStack_168);
    return;
}
