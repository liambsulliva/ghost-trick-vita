extern unsigned int data_02099d14[];

extern char data_020cf7c8;
extern int func_02008444();
extern int func_0200847c();
extern int func_02009900();
extern int func_02010d18();
extern int func_02010ef4();
extern int func_0202a0c0();
extern int func_0202fad4();
extern int func_02031b88();

void func_020318c4(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02009900();
    func_0202fad4(uVar1,((unsigned int)&data_020cf7c8));
    *(unsigned int *)(param_1 + 0x2c) = 0xffffffff;
    *(unsigned int *)(param_1 + 0x30) = 0xffffffff;
    *(unsigned int *)(param_1 + 0x34) = 0xffffffff;
    if (*(int *)(param_1 + 8) == 1) {
        *(unsigned int *)(param_1 + 0x38) = 0;
        *(unsigned char *)(param_1 + 4) = 2;
    }
    else {
        func_02031b88(param_1);
    }
    uVar1 = func_0200847c();
    func_0202a0c0(uVar1,data_02099d14[(*(int *)(param_1 + 8)) * 1]);
    func_02008444();
    func_02010d18();
    uVar1 = func_02008444();
    func_02010ef4(uVar1,0xff,0xffffffff);
    return;
}
