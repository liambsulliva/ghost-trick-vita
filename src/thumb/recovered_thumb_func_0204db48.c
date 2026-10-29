extern int func_020089fc();
extern int func_02008a00();
extern int func_02008a64();
extern int func_020471b0();

void func_0204db48(int param_1,unsigned int param_2)

{
    unsigned char auStack_48 [56];

    func_020471b0();
    func_02008a64(auStack_48,param_1,((unsigned int)0x0209bbdcu),param_1 + 0xb8,1,0);
    func_02008a00(param_2,auStack_48);
    func_020089fc(auStack_48);
    return;
}
