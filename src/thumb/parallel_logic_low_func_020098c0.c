extern char data_020b42bc;
extern char data_020b42d8;
extern unsigned int func_02009900();
extern void func_0202fad4();
extern void func_020088b4();
extern void func_0207de48();
extern void func_0200a68c();

int func_020098c0(int param_1)
{
    unsigned int uVar1;

    uVar1 = func_02009900();
    func_0202fad4(uVar1, (unsigned int)&data_020b42bc);
    uVar1 = func_02009900();
    func_0202fad4(uVar1, (unsigned int)&data_020b42d8);
    func_020088b4(param_1 + 0xaa04);
    func_0207de48(param_1, 0x10, 0x10, (unsigned int)func_0200a68c);
    return param_1;
}
