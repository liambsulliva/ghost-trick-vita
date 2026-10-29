extern char data_020b18a0;
extern int func_02003d8c();

unsigned int func_02003d2c(void)

{
    int iVar1;

    iVar1 = func_02003d8c();
    if (iVar1 == 0x10) {
        return 0xffffffff;
    }
    return *(unsigned int *)(*(int *)(((unsigned int)&data_020b18a0) + 0x10) + iVar1 * 0x4c + ((unsigned int)0x00001604u));
}
