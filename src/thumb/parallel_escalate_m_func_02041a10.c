extern char data_020a7c00;
typedef int code();
extern int func_02007b70();
extern unsigned char * func_02007bd8();
extern unsigned char * func_02022d64();

unsigned int * func_02041a10(unsigned int *param_1)

{
    unsigned char *puVar1;
    int iVar2;
    int *piVar3;

    *param_1 = ((unsigned int)&data_020a7c00);
    if (((((unsigned int)*(unsigned char *)((int)param_1 + 0x51)) << 29) >> 31) == 0 && (param_1[0x12] != 0)) {
        puVar1 = func_02007bd8();
        iVar2 = func_02007b70(puVar1);
        if (iVar2 != 0) {
            puVar1 = func_02007bd8();
            piVar3 = (int *)func_02007b70(puVar1);
            (**(code **)(*piVar3 + 0x10))(piVar3,param_1[0x12]);
            param_1[0x12] = 0;
        }
    }
    func_02022d64((unsigned char *)param_1);
    return param_1;
}
