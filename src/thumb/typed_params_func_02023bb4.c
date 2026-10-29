extern char data_020c9e04;
extern char data_020c9e2c;
extern int func_0201ee30();
extern int func_0201ee8c();

void func_02023bb4(int param_1)

{
    unsigned char *puVar1;
    int iVar2;

    puVar1 = (unsigned char *)func_0201ee30(param_1,((unsigned int)&data_020c9e2c));
    if (puVar1 != (unsigned char *)0x0) {
        iVar2 = func_0201ee8c(puVar1);
        *(int *)(((int)&data_020c9e04) + 4) = iVar2;
    }
    return;
}
