extern int func_02069fec();
extern int func_0206a97c();
extern int func_0206aa3c();
extern int func_0206b168();

void func_0206ad20(void)

{
    unsigned int *puVar1;
    int iVar2;

    iVar2 = func_0206a97c();
    func_0206b168(iVar2);
    func_0206aa3c(8);
    puVar1 = ((unsigned int *)0x020d02a0u);
    *(unsigned int *)*((unsigned int *)0x020d02a0u) = 0;
    func_02069fec((int)puVar1);
    return;
}
