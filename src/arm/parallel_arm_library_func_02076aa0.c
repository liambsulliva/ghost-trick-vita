extern int func_02076fe4();
extern int *func_02076f9c();
extern int func_02076ed0();
extern int func_020769a0();
extern void func_02076b60(int zero);
extern void func_02076f74();

int *func_02076aa0(unsigned int param_1)
{
    int *piVar1;

    if (func_02076fe4() == 0) {
        return (int *)0;
    }
    piVar1 = func_02076f9c();
    if (piVar1 != (int *)0) {
        return piVar1;
    }
    if ((param_1 & 1) == 0) {
        return (int *)0;
    }
    if (func_02076ed0() > 0) {
        do {
            piVar1 = (int *)func_020769a0(0);
        } while (piVar1 != (int *)0);
        piVar1 = func_02076f9c();
        if (piVar1 != (int *)0) {
            return piVar1;
        }
    }
    else {
        func_02076b60(1);
    }
    func_02076f74();
    do {
        func_020769a0(1);
        piVar1 = func_02076f9c();
    } while (piVar1 == (int *)0);
    return piVar1;
}
