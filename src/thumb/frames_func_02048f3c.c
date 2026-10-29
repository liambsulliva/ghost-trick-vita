extern char data_0209b848;
extern char data_0209b850;
extern int func_020089c8();
extern void func_020089fc();
extern int func_02008a00();
extern int func_0200b374();
extern int func_020471b0();

void func_02048f3c(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_50 [56];
    unsigned char auStack_88 [56];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020471b0();
    func_0200b374(auStack_50,param_1,((unsigned int)&data_0209b848),param_1 + 0xbc,1,0);
    func_02008a00(param_2,auStack_50);
    func_020089fc(auStack_50);
    func_020089c8(auStack_88,param_1,((unsigned int)&data_0209b850),param_1 + 0xc0,1,0);
    func_02008a00(param_2,auStack_88);
    func_020089fc(auStack_88);
    return;
}
