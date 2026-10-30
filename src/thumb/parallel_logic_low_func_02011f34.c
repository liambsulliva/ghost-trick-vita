extern int func_02011e00();
extern int func_02011e04();
extern int func_02011f90();
extern unsigned char * func_02011f98();
extern unsigned char * func_02031254();

void func_02011f34(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)
{
    unsigned char bVar1;
    unsigned char *puVar3;

    if (*(unsigned char *)(param_1 + 0xf20) < *(unsigned char *)(param_1 + 0xf21)) {
        puVar3 = (unsigned char *)func_02031254(0x1c);
        if (puVar3 != (unsigned char *)0x0) {
            puVar3 = func_02011f98(puVar3);
        }
        if (puVar3 != (unsigned char *)0x0) {
            func_02011f90(puVar3,param_4,param_5,param_6);
            func_02011e00(puVar3,param_2);
            func_02011e04(puVar3,param_3);
            bVar1 = *(unsigned char *)(param_1 + 0xf20);
            *(unsigned char *)(param_1 + 0xf20) = bVar1 + 1;
            *(unsigned char **)(param_1 + (unsigned int)bVar1 * 4 + 0xf24) = puVar3;
        }
    }
    return;
}
