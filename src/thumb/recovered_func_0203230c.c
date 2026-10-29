extern int func_02009900();
extern int func_0202fa8c();

unsigned int func_0203230c(void)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02009900();
    iVar2 = func_0202fa8c(uVar1,((unsigned int)0x020cfb28u));
    if (iVar2 == 0) {
        return 1;
    }
    return 0;
}
