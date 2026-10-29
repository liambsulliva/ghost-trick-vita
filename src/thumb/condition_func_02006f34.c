extern char data_020b18d8;
extern int func_02001b54();

int func_02006f34(void)

{
    int *piVar1;
    int iVar2;

    iVar2 = ((int)0x00001b7du);
    piVar1 = ((int *)&data_020b18d8);
    *(unsigned char *)(*((int *)&data_020b18d8) + ((int)0x00001b7du)) = 1;
    iVar2 = func_02001b54(*(unsigned int *)(*piVar1 + iVar2 + 0x1f));
    if (iVar2 + 3U <= 1) {
        iVar2 = -3;
    }
    return iVar2;
}
