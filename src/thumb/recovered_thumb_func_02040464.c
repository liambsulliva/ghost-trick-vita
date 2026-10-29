extern char data_020ce320;
extern char data_020ce348;
extern char data_020ce39c;
extern int func_020082f8();
extern int func_02008444();
extern int func_02009900();
extern int func_0202f64c();
extern int func_0202fad4();
extern int func_020356e0();
extern int func_020405d8();

void func_02040464(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_020082f8(*(unsigned int *)(param_1 + 0x20),((unsigned int)&data_020ce348));
    if (iVar1 != 0) {
        func_020405d8();
    }
    func_0202f64c(param_1 + 0x20);
    uVar2 = func_02008444();
    func_020356e0(uVar2,*(unsigned int *)(param_1 + 0x24));
    if (*(char *)(((int)&data_020ce320) + 1) != '\0') {
        uVar2 = func_02009900();
        func_0202fad4(uVar2,((unsigned int)&data_020ce39c));
    }
    return;
}
