extern unsigned char * func_0206884c();
extern int func_02072ec4();

unsigned char * func_02068864(int param_1)
{
    unsigned int *puVar1;
    int iVar2;

    puVar1 = (unsigned int *)func_0206884c((unsigned char *)(param_1 + 0x24));
    if (puVar1 != (unsigned int *)0x0)
    {
        iVar2 = *(int *)(param_1 + 0x28);
        if (((unsigned char)*(unsigned int *)(param_1 + 0x20) & 1) != 0)
        {
            func_02072ec4(0,puVar1,iVar2);
        }
    }
    return (unsigned char *)puVar1;
}
