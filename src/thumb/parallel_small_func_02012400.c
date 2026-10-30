extern unsigned char * func_0200d7e4();
extern int func_0200d80c();
extern int func_0200d818();
extern unsigned char * func_0200d8a8();

void func_02012400(int param_1,int param_2,int param_3)

{
    int iVar1;
    unsigned char *puVar2;
    unsigned char *puVar3;

    if (*(unsigned int *)(param_1 + ((int)0x00000edcu)) < 0x20) {
        puVar2 = func_0200d8a8(0x10);
        puVar3 = puVar2;
        if (puVar2 != (unsigned char *)0x0) {
            puVar3 = func_0200d7e4(puVar2);
        }
        if (puVar3 != (unsigned char *)0x0) {
            func_0200d80c(puVar3,param_2);
            func_0200d818(puVar3,param_3);
            iVar1 = ((int)0x00000edcu);
            *(unsigned char **)(param_1 + *(int *)(param_1 + ((int)0x00000edcu)) * 4 + ((int)0x00000edcu) + -0x80) =
                      puVar3;
            *(int *)(param_1 + iVar1) = *(int *)(param_1 + iVar1) + 1;
        }
    }
    return;
}
