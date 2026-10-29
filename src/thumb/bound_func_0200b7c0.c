extern char data_020cff60;
extern int func_0200b7b0();
extern int func_0200b804();
extern int func_020695c8();

unsigned int func_0200b7c0(int param_1)

{
    unsigned short uVar1;
    int iVar2;

    iVar2 = func_0200b804(param_1 + 0x54);
    if ((iVar2 != 0) &&
          (iVar2 = func_020695c8(param_1 + 0x54,(*(unsigned char *)(((int)&data_020cff60) + 1) + 1) * 0x1000), iVar2 != 0))
    {
        uVar1 = func_0200b7b0(param_1 + 0x54);
        *(unsigned short *)(param_1 + 0x28) = uVar1;
    }
    iVar2 = func_0200b804(param_1 + 0x54);
    if (iVar2 == 0) {
        return 0;
    }
    return 1;
}
