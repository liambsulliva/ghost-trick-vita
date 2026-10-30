typedef int code();

extern int func_02008860();
extern int func_0200c820();

int func_02058a10(int param_1)
{
    int *piVar1;

    func_02008860();
    piVar1 = (int *)func_0200c820();
    if (piVar1 != (int *)0x0) {
        return (*(code **)(*piVar1 + 8))((int)piVar1, 0x18, 0x0209d938);
    }
    return 0;
}
