extern int func_0200adac();
extern int func_0200add0();
extern int func_0200addc();
extern int func_0200ade8();
extern int func_0206f6a8();

void func_0200ad58(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_4c [64];
    unsigned int uStack_c;

    uStack_c = param_4;
    func_0200adac(0x60000,((unsigned int)0xfffa0000u),((unsigned int)0xfff80000u),0x80000,*(unsigned int *)(param_1 + 4),
                                *(unsigned int *)(param_1 + 8),0);
    func_0200add0(0);
    func_0200addc();
    func_0200add0(3);
    func_0200ade8(auStack_4c);
    func_0206f6a8(auStack_4c);
    func_0200add0(2);
    func_0200addc();
    return;
}
