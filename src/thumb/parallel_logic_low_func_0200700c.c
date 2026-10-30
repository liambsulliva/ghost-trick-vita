extern char data_020b18d8;
extern int func_02002564();

int func_0200700c(unsigned int param_1)
{
    int iVar2;

    iVar2 = 0x1b7d;
    *(unsigned char *)(*(int *)&data_020b18d8 + iVar2) = 1;
    iVar2 = func_02002564(*(unsigned int *)(*(int *)&data_020b18d8 + iVar2 + 0x1f), param_1);
    if (iVar2 + 3U <= 1) {
        iVar2 = -3;
    }
    return iVar2;
}
