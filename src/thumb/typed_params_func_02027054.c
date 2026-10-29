extern int func_02008828();
extern int func_0205953c();

void func_02027054(unsigned int param_1,unsigned char *param_2,int param_3)

{
    int iVar1;

    if ((param_2 != (unsigned char *)0x0) && (iVar1 = func_02008828(param_2,((unsigned char *)0x020cf898u)), iVar1 != 0))
    {
        func_0205953c((int)param_2,*(unsigned short *)(param_3 + 4) & 0xff);
    }
    return;
}
