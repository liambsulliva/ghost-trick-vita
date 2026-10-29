extern int func_020760c8();
extern int func_020761d0();

void func_0206b05c(void)

{
    unsigned int *puVar1;
    unsigned int uVar2;

    func_020760c8();
    puVar1 = ((unsigned int *)0x020d0ba0u);
    *((unsigned int *)0x020d0ba0u) = 0;
    uVar2 = ((unsigned int)0x0206b090u);
    puVar1[1] = 0;
    func_020761d0(0xe,uVar2);
    puVar1[2] = 0;
    return;
}
