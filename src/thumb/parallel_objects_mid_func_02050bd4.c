extern unsigned char * func_02012574();
extern int func_02050900();

unsigned int func_02050bd4(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    if (*(int *)(param_1 + 0x40) == 0) {
        return 0;
    }
    {
        uVar1 = 0;
        uStack_18 = param_4;
        func_02012574(auStack_28,param_2,param_3);
        for (iVar3 = *(int *)(*(int *)(param_1 + 0x40) + 0x54); iVar3 != 0;
                iVar3 = *(int *)(iVar3 + 0x54)) {
            iVar2 = func_02050900(iVar3,(int)auStack_28);
            if (iVar2 != 0) {
                uVar1 = 1;
            }
        }
    }
    return uVar1;
}
