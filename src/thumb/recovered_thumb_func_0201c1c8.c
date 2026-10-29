extern char data_020b9d14;
extern char data_020cff60;
int func_0201c1c8(unsigned int param_1,unsigned int param_2)

{
    char cVar1;
    int iVar2;

    iVar2 = (param_2 - *(unsigned short *)(((int)&data_020b9d14) + 2)) * 5 + ((int)0x000002e1u);
    cVar1 = *((char *)&data_020cff60);
    if ((((cVar1 == '\x02') && (0x78 < param_2)) || ((cVar1 == '\x03' && (0xa8 < param_2)))) ||
          (((cVar1 == '\x04' && (0xd8 < param_2)) || ((cVar1 == '\x05' && (0x108 < param_2)))))) {
        iVar2 = iVar2 + -5;
    }
    if ((((param_2 == 0x78) || (param_2 == 0xa8)) || (param_2 == 0xd8)) || (param_2 == 0x108)) {
        iVar2 = ((int)0x00000408u);
    }
    return iVar2;
}
