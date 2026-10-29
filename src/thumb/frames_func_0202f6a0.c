extern char data_02085658;
extern char data_02085660;
extern char data_02085668;
extern char data_02085670;
extern char data_02085680;
extern int func_020089c8();
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_02008ad4();
extern int func_02012ea8();

void func_0202f6a0(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned char auStack_130 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_02008ad4();
    func_02012ea8(auStack_50,param_1,((unsigned int)&data_02085658),param_1 + 0xd,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02012ea8(auStack_88,param_1,((unsigned int)&data_02085660),param_1 + 0xe,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_020089c8(auStack_c0,param_1,((unsigned int)&data_02085668),param_1 + 0x10,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_02008a64(auStack_f8,param_1,((unsigned int)&data_02085670),param_1 + 0x20,1,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    func_02008a64(auStack_130,param_1,((unsigned int)&data_02085680),param_1 + 0x24,1,0);
    func_02008a00(param_2,auStack_130);
    func_020089fc(auStack_130);
    return;
}
