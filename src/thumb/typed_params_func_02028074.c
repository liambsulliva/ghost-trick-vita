extern int func_02008828();
extern int func_0202809c();
extern unsigned char * func_02028148();

void func_02028074(unsigned char *param_1,unsigned int param_2)

{
    unsigned char *puVar1;
    int iVar2;

    puVar1 = func_02028148(param_1);
    if ((puVar1 != (unsigned char *)0x0) && (iVar2 = func_02008828(puVar1,((unsigned char *)0x020cb04cu)), iVar2 != 0)) {
        func_0202809c(puVar1,param_2 & 0xff);
    }
    return;
}
