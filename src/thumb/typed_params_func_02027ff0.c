extern int func_02008828();
extern int func_0202801c();
extern unsigned char * func_02028148();

void func_02027ff0(unsigned char *param_1,unsigned short param_2,unsigned short param_3,unsigned char param_4)

{
    unsigned char *puVar1;
    int iVar2;

    puVar1 = func_02028148(param_1);
    if ((puVar1 != (unsigned char *)0x0) && (iVar2 = func_02008828(puVar1,((unsigned char *)0x020cf8b4u)), iVar2 != 0)) {
        func_0202801c((int)puVar1,param_2,param_3,param_4);
    }
    return;
}
