extern char data_0209d93c;
extern char data_0209d948;
extern char data_0209d950;
extern int func_020089c8();
extern void func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_02012f14();

void func_02058ad0(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned char auStack_c0 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_02008a64(auStack_50,param_1,((unsigned int)&data_0209d93c),param_1 + 4,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_02012f14(auStack_88,param_1,((unsigned int)&data_0209d948),param_1 + 8,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    func_020089c8(auStack_c0,param_1,((unsigned int)&data_0209d950),param_1 + 0x14,1,0);
    func_02008a00(param_2,auStack_c0);
    func_020089fc(auStack_c0);
    return;
}
