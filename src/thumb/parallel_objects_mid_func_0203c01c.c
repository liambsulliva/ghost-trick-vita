extern unsigned int *func_02008444();
extern int func_0201132c();

unsigned int func_0203c01c(void)
{
    unsigned int *puVar1;
    int iVar2;

    puVar1 = func_02008444();
    iVar2 = func_0201132c((int)puVar1);
    if (iVar2 != 0) {
        return 0x020cd96cu;
    }
    return 0x020cd7ecu;
}
