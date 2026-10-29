extern int func_0200847c();
extern int func_0202a0c0();
extern int func_0202f25c();
extern int func_ov004_020d7050();

void func_ov004_020d712c(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_0202f25c();
    func_ov004_020d7050(uVar1,((unsigned int)0x020e4660u),0xffffffff);
    uVar1 = func_0200847c();
    func_0202a0c0(uVar1,0x52);
    *(unsigned char *)(param_1 + 0x25) = 1;
    return;
}
