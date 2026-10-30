extern unsigned char data_020c9e88[];
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0202f64c();

void func_ov003_020d3ad8(int param_1)
{
    int iVar1;
    int *piVar2;
    int iVar3;

    func_0202f64c(param_1 + 0x20);
    func_0202f64c(param_1 + 0x24);
    func_0202f64c(param_1 + 0x28);
    func_0202f64c(param_1 + 0x2c);
    func_0202f64c(param_1 + 0x30);
    iVar3 = ((unsigned int)0x0000016bu);
    if ((*(char *)(param_1 + ((unsigned int)0x0000016bu)) == '\0') &&
          (*(int *)(param_1 + ((unsigned int)0x0000016bu) + 0x19) != 0)) {
        iVar1 = func_02007b70(func_02007bd8());
        if (iVar1 != 0) {
            piVar2 = (int *)func_02007b70(func_02007bd8());
            (**(code **)(*piVar2 + 0x10))(piVar2,*(unsigned int *)(param_1 + iVar3 + 0x19));
            *(unsigned int *)(param_1 + iVar3 + 0x19) = 0;
        }
    }
    iVar3 = 0;
    do {
        func_0202f64c(param_1 + 0x14c + iVar3 * 4);
        iVar1 = 0;
        do {
            func_0202f64c(param_1 + 0x11c + iVar3 * 0x18 + iVar1 * 4);
            iVar1 = iVar1 + 1;
        } while (iVar1 < 6);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 2);
    iVar3 = 0;
    do {
        iVar1 = iVar3 * 4;
        func_0202f64c(param_1 + 0x44 + iVar1);
        func_0202f64c(param_1 + 0x8c + iVar1);
        func_0202f64c(param_1 + 0xd4 + iVar1);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 0x12);
    func_0202f64c(param_1 + 0x34);
    func_0202f64c(param_1 + 0x38);
    iVar3 = 0;
    do {
        func_0202f64c(param_1 + 0x3c + iVar3 * 4);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 2);
    func_0202f64c(param_1 + 0x154);
    iVar3 = 0;
    do {
        func_0202f64c(param_1 + 0x158 + iVar3 * 4);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 2);
    iVar3 = 0;
    do {
        func_0202f64c(param_1 + 0x160 + iVar3 * 4);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 2);
    *(unsigned short *)(data_020c9e88 + 0x2e) = 0;
    return;
}
