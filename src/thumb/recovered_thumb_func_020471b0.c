extern int func_020089fc();
extern int func_02008a00();
extern int func_0200b340();
extern int func_020527c8();

void func_020471b0(int param_1,unsigned int param_2)

{
    unsigned char auStack_48 [56];

    func_020527c8();
    func_0200b340(auStack_48,param_1,((unsigned int)0x0209b7e8u),param_1 + 0xb6,1,0);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
