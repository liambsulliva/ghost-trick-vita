typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02009c1c();
extern int func_0202f1bc();
extern int func_020648a8();

void func_02064858(unsigned int param_1,int param_2)

{
    int *piVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    func_02007bd8();
    piVar1 = (int *)func_02007b70();
    uVar2 = (**(code **)(*piVar1 + 0xc))(piVar1,0x1000);
    func_020648a8(uVar2,0x1000);
    if (param_2 == 0) {
        uVar3 = func_02009c1c();
        func_0202f1bc(uVar3,uVar2,0,0x1000,0xf);
        return;
    }
    if (param_2 == 1) {
        uVar3 = func_02009c1c();
        func_0202f1bc(uVar3,uVar2,0,0x1000,0x1b);
    }
    return;
}
