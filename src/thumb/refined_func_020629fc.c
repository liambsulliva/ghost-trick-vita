extern char data_0209de54;
extern char data_0209de60;
extern char data_0209de6c;
extern char data_0209de78;
extern char data_0209de84;
extern char data_0209de90;
extern char data_0209dea0;
extern char data_0209deb0;
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a30();
extern int func_02008a64();
extern int func_0200b340();
extern int func_0200b374();
extern int func_02012ea8();
extern int func_02056e1c();

void func_020629fc(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

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
    func_02056e1c();
    func_02008a64(auStack_50,param_1,((unsigned int)&data_0209de60),param_1 + 0x7c,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02012ea8(auStack_88,param_1,((unsigned int)&data_0209de6c),param_1 + 0x98,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_02012ea8(auStack_c0,param_1,((unsigned int)&data_0209de78),param_1 + 0x99,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_02012ea8(auStack_f8,param_1,((unsigned int)&data_0209de84),param_1 + 0x9a,1,0);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    func_0200b340(auStack_130,param_1,((unsigned int)&data_0209de90),param_1 + 0x96,1,0);
    func_02008a00(param_2,auStack_130);
    func_020089fc(auStack_130);
    func_0200b374(auStack_168,param_1,((unsigned int)&data_0209dea0),param_1 + 0x80,1,0);
    func_02008a00(param_2,auStack_168);
    func_020089fc(auStack_168);
    func_0200b374(auStack_1a0,param_1,((unsigned int)&data_0209deb0),param_1 + 0x84,1,0);
    func_02008a00(param_2,auStack_1a0);
    func_020089fc(auStack_1a0);
    func_02008a30(auStack_1d8,param_1,((unsigned int)&data_0209de54),param_1 + 0x94,1,0);
    func_02008a00(param_2,auStack_1d8);
    func_020089fc(auStack_1d8);
    return;
}
