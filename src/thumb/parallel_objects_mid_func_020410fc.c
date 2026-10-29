extern unsigned char * func_02009900();
extern int func_0202f7e8();
extern unsigned char * func_020402b0();
extern int func_020406d8();
extern int func_020410ec();

unsigned short * func_020410fc(unsigned short *param_1,int param_2)

{
    unsigned char *puVar1;

    *param_1 = 0;
    param_1[1] = 0;
    *(unsigned int *)(param_1 + 2) = 0;
    if (param_2 == 0) {
        param_1[1] = (short)((unsigned int)0x000012f1u);
    }
    else {
        param_1[1] = (short)param_2;
    }
    puVar1 = func_020402b0(((unsigned char *)0x020ce3d4u));
    *(unsigned char **)(param_1 + 4) = puVar1;
    if (puVar1 != (unsigned char *)0x0) {
        func_020406d8(puVar1,(unsigned int)(unsigned short)param_1[1],8);
        func_020410ec(*(int *)(param_1 + 4),1);
        puVar1 = func_02009900();
        func_0202f7e8((int)puVar1,*(int *)(param_1 + 4),10,0);
        *(unsigned int *)(param_1 + 2) = *(unsigned int *)(param_1 + 4);
    }
    puVar1 = func_020402b0(((unsigned char *)0x020ce3d4u));
    *(unsigned char **)(param_1 + 6) = puVar1;
    if (puVar1 != (unsigned char *)0x0) {
        func_020406d8(puVar1,(unsigned int)(unsigned short)param_1[1],8);
        func_020410ec(*(int *)(param_1 + 6),0);
        puVar1 = func_02009900();
        func_0202f7e8((int)puVar1,*(int *)(param_1 + 6),10,0);
    }
    *param_1 = 2;
    return param_1;
}
