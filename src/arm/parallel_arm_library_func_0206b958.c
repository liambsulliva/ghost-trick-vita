extern unsigned int func_02075c98();
extern void func_02075cac();
extern void func_020740d0();
extern int func_0206b3a8();
extern void func_0206b2ac();
extern unsigned int *func_0206b728();
extern void func_0206b8d4();

void func_0206b958(unsigned int *param_1)
{
    unsigned int handle;
    unsigned int *puVar2;
    int iVar1;
    unsigned int iVar3;

    handle = func_02075c98();
    while (((param_1[3] & 0x40) == 0) && ((param_1[3] & 1) != 0))
    {
        func_020740d0(param_1 + 6);
    }
    func_02075cac(handle);
    if ((param_1[3] & 0x40) == 0)
    {
        return;
    }
    iVar3 = param_1[2];
    iVar1 = func_0206b3a8(param_1, (*(volatile unsigned int *)(param_1 + 3) >> 8) & 0xff);
    func_0206b2ac(param_1, iVar1);
    puVar2 = func_0206b728(iVar3, 1);
    if (puVar2 == (unsigned int *)0)
    {
        return;
    }
    func_0206b8d4(puVar2);
}
