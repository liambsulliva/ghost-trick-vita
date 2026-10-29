typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02008444();
extern int func_02010384();
extern int func_0202f64c();
extern int func_020312e4();
extern int func_0203cbe0();

void func_0203cc20(int param_1)

{
    int iVar1;
    int *piVar2;
    unsigned int uVar3;
    unsigned int uVar4;

    func_0202f64c(param_1 + 0x24);
    func_0202f64c(param_1 + 0x28);
    func_0202f64c(param_1 + 0x2c);
    func_0202f64c(param_1 + 0x5c);
    uVar4 = 0;
    do {
        func_0202f64c(param_1 + 0x38 + uVar4 * 4);
        uVar4 = uVar4 + 1;
    } while (uVar4 < 2);
    func_0202f64c(param_1 + 0x30);
    func_0202f64c(param_1 + 0x34);
    func_0202f64c(param_1 + 0x20);
    if (*(int *)(param_1 + 0x40) != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,*(unsigned int *)(param_1 + 0x40));
            *(unsigned int *)(param_1 + 0x40) = 0;
        }
    }
    uVar3 = func_020312e4();
    func_0203cbe0(uVar3,0);
    uVar3 = func_02008444();
    func_02010384(uVar3,0);
    return;
}
