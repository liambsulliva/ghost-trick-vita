extern int *func_020312e4();
extern int *func_0200847c();
extern int *func_02008444();
void func_020317c8(void)
{
    int iVar2;
    int *piVar3;

    iVar2 = func_0203187c(0x020c9d50);
    if (iVar2 == 0) {
        return;
    }
    piVar3 = func_020312e4();
    iVar2 = func_020317b0(piVar3);
    if (iVar2 != 0) {
        return;
    }
    piVar3 = func_020312e4();
    iVar2 = func_020317b4(piVar3);
    if (iVar2 != 0) {
        return;
    }
    piVar3 = func_0200847c();
    func_0202a770(piVar3);
    piVar3 = func_0200847c();
    func_0202a01c(piVar3);
    piVar3 = func_02008444();
    iVar2 = func_020317b8(piVar3);
    if (iVar2 != 0) {
        func_0202f25c();
        func_ov004_020d5978();
    }
    piVar3 = func_0200847c();
    func_0202a1b0(piVar3, 0x52, 0);
    piVar3 = func_0200847c();
    func_0202a1b0(piVar3, 0x1eb, 0);
    func_020747e4();
    func_02078788(0xc, 0, 0);
    do {
        iVar2 = func_02078c74(1);
    } while (iVar2 == 0);
    piVar3 = func_02008444();
    iVar2 = func_020317b8(piVar3);
    if (iVar2 != 0) {
        func_0202f25c();
        func_ov004_020d5978();
    }
    func_02075d14(0x7fd87f);
    piVar3 = func_0200847c();
    func_0202a770(piVar3);
    piVar3 = func_0200847c();
    func_0202a01c(piVar3);
}
