extern int func_0200847c();
extern int func_0202a068();

void func_02012110(int param_1,unsigned int param_2)

{
    unsigned int uVar1;

    *(unsigned short *)(param_1 + 0x14) = 0;
    *(unsigned short *)(param_1 + 0x16) = 0;
    uVar1 = func_0200847c();
    func_0202a068(uVar1,param_2);
    return;
}
