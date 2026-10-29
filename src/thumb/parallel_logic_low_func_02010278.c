extern unsigned char * func_02009900();
extern int func_0202fb48();
extern int func_02046f0c();

int func_02010278(void)

{
    unsigned char *puVar1;
    int iVar2;

    puVar1 = func_02009900();
    iVar2 = func_0202fb48(puVar1,((unsigned char *)0x020ce7d4u),1);
    if (iVar2 != 0) {
        return func_02046f0c(iVar2);
    }
    return 4;
}
