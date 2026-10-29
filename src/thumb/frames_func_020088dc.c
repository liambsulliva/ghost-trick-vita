extern char data_02083a34;
extern char data_02083a3c;
extern char data_02083a4c;
extern char data_02083a5c;
extern char data_02083a64;
extern char data_02083a70;
extern int func_020089c8();
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_02008a64();

void func_020088dc(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned char auStack_130 [56];
    unsigned char auStack_168 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020089c8(auStack_50,param_1,((unsigned int)&data_02083a34),param_1 + 8,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a30(auStack_88,param_1,((unsigned int)&data_02083a3c),param_1 + 0xe,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_02008a30(auStack_c0,param_1,((unsigned int)&data_02083a4c),param_1 + 0x10,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_02008a30(auStack_f8,param_1,((unsigned int)&data_02083a5c),param_1 + 0x12,1,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    func_02008a64(auStack_130,param_1,((unsigned int)&data_02083a64),param_1 + 0x14,1,0);
    func_02008a00(param_2,auStack_130);
    func_020089fc(auStack_130);
    func_02008a64(auStack_168,param_1,((unsigned int)&data_02083a70),param_1 + 0x28,1,0);
    func_02008a00(param_2,auStack_168);
    func_020089fc(auStack_168);
    return;
}
