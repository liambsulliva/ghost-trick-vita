typedef int code();
extern int func_0202f64c();

void func_ov006_020d414c(int param_1)

{
    int *piVar1;
    unsigned int uVar2;
    int iVar3;

    func_0202f64c(param_1 + 0x4c);
    func_0202f64c(param_1 + 0x50);
    func_0202f64c(param_1 + 0x54);
    func_0202f64c(param_1 + 0x58);
    func_0202f64c(param_1 + 0x5c);
    func_0202f64c(param_1 + 0x60);
    func_0202f64c(param_1 + 100);
    uVar2 = 0;
    do {
        iVar3 = param_1 + uVar2 * 4;
        piVar1 = *(int **)(iVar3 + 0x68);
        if (piVar1 != (int *)0x0) {
            (**(code **)(*piVar1 + 0x20))(piVar1,param_1);
            *(unsigned int *)(iVar3 + 0x68) = 0;
        }
        uVar2 = uVar2 + 1;
    } while (uVar2 < 2);
    uVar2 = 0;
    do {
        func_0202f64c(param_1 + 0x70 + uVar2 * 4);
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 9);
    func_0202f64c(param_1 + 0x94);
    return;
}
