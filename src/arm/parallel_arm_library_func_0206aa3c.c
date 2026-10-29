extern int data_020d0f18;
extern int func_0206a9d4();
extern int func_0206aa9c();
extern int func_020742a0();
extern int func_020754e4();

void func_0206aa3c(unsigned int param_1)

{
    unsigned int uVar2;

    uVar2 = func_0206a9d4();
    if ((param_1 & uVar2) == 0) {
        return;
    }
    func_0206aa9c();
    uVar2 = func_0206a9d4();
    if ((uVar2 & 0x20) != 0) {
        return;
    }
    do {
        if ((data_020d0f18 != 0) && (uVar2 = func_020754e4(), uVar2 != 0)) {
            func_020742a0(1);
        }
        uVar2 = func_0206a9d4();
    } while ((uVar2 & 0x20) == 0);
    return;
}
