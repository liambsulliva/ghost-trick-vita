extern char *func_02027c28(unsigned int);
extern void func_0200da04();
extern char data_0209c1c8[];

int func_02051adc(int param_1)
{
    char *pcVar1;
    char *puVar2;
    char *pcVar3;
    int iVar4;

    iVar4 = *(int *)(param_1 + 4) + *(int *)(param_1 + 8) * 2;
    pcVar1 = func_02027c28((unsigned int)*(unsigned short *)(iVar4 + 2));
    puVar2 = func_02027c28((unsigned int)*(unsigned short *)(iVar4 + 4));
    pcVar3 = func_02027c28((unsigned int)*(unsigned short *)(iVar4 + 6));
    func_0200da04(pcVar1, (int)puVar2, pcVar3, (int)data_0209c1c8);
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 4;
    return 1;
}
