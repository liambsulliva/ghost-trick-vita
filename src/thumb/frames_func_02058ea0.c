extern char data_0209d93c;
extern char data_0209d950;
extern char data_0209d95c;
extern char data_0209d968;
extern char data_0209d974;
extern char data_0209d984;
extern char data_0209d990;
extern int func_020089c8();
extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_02012ea8();
extern int func_02012f14();
extern int func_0202feb4();

void func_02058ea0(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned char auStack_f8 [56];
    unsigned char auStack_130 [56];
    unsigned char auStack_168 [56];
    unsigned char auStack_1a0 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202feb4();
    func_02012ea8(auStack_50,param_1,((unsigned int)&data_0209d95c),param_1 + 0xbe,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02008a64(auStack_88,param_1,((unsigned int)&data_0209d968),param_1 + 0xc4,0x21,4);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_02008a64(auStack_c0,param_1,((unsigned int)&data_0209d93c),param_1 + 0xd8,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    func_02012f14(auStack_f8,param_1,((unsigned int)&data_0209d974),param_1 + 0xe4,0x21,4);
    func_02008a00(param_2,auStack_f8);
    func_020089fc(auStack_f8);
    func_02012ea8(auStack_130,param_1,((unsigned int)&data_0209d984),param_1 + 0xc3,1,0);
    func_02008a00(param_2,auStack_130);
    func_020089fc(auStack_130);
    func_020089c8(auStack_168,param_1,((unsigned int)&data_0209d950),param_1 + 0xdc,1,0);
    func_02008a00(param_2,auStack_168);
    func_020089fc(auStack_168);
    func_02012ea8(auStack_1a0,param_1,((unsigned int)&data_0209d990),param_1 + 0xc2,1,0);
    func_02008a00(param_2,auStack_1a0);
    func_020089fc(auStack_1a0);
    return;
}
