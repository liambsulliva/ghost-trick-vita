extern char data_020ce900;
extern char data_020cedc4;
extern char data_020cee88;
extern char data_020cfe48;
extern int func_02008828();
extern int func_02009900();
extern int func_0202f7e8();

void func_02012fe4(unsigned int param_1,unsigned int param_2)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_02008828(param_2,((unsigned int)&data_020cedc4));
    if (iVar1 != 0) {
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,param_2,4,1);
        return;
    }
    iVar1 = func_02008828(param_2,((unsigned int)&data_020cee88));
    if (iVar1 != 0) {
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,param_2,5,1);
        return;
    }
    iVar1 = func_02008828(param_2,((unsigned int)&data_020cfe48));
    if (iVar1 != 0) {
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,param_2,7,1);
        return;
    }
    iVar1 = func_02008828(param_2,((unsigned int)&data_020ce900));
    if (iVar1 != 0) {
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,param_2,7,1);
    }
    return;
}
