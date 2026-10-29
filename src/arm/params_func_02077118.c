extern int func_02074844();

unsigned int func_02077118(int param_1)

{
    int *piVar1;

    piVar1 = ((int *)0x020d2e80u);
    func_02074844(*((int *)0x020d2e80u) + 0x40 + param_1 * 0x24,4);
    return *(unsigned int *)(*piVar1 + param_1 * 0x24 + 0x40);
}
