extern unsigned int * func_02008444();
extern unsigned char * func_0201123c();
extern unsigned char * func_02011264();
extern int func_02011340();
extern int func_0201135c();
extern int func_0201572c();
extern int func_02022f34();
extern int func_0203c6a8();

void func_0203c570(unsigned char *param_1)

{
    int iVar1;
    unsigned char *puVar2;
    unsigned int *puVar3;

    iVar1 = func_0203c6a8(param_1,0,0,1);
    if (((iVar1 == 0) && ((unsigned char)(param_1[4] + 0xffU) <= 2)) && (iVar1 = func_0201572c(), iVar1 == 0)) {
        if ((unsigned char)(param_1[4] + 0xffU) <= 1) {
            puVar2 = func_02011264();
            puVar2 = func_0201123c(puVar2,0);
            func_02022f34((int)puVar2,((int)0x0209aaf4u),0);
            puVar3 = func_02008444();
            func_02011340((unsigned char *)puVar3);
            puVar3 = func_02008444();
            func_0201135c((int)puVar3);
            return;
        }
        param_1[4] = 0xc;
        *(unsigned short *)(param_1 + 0x4a) = 0;
        param_1[0x55] = 1;
    }
    return;
}
