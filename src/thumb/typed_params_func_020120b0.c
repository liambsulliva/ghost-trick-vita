extern unsigned char * func_0200847c();
extern int func_020120f0();
extern int func_02012100();
extern int func_0202a044();

void func_020120b0(int param_1,unsigned int param_2,int param_3,unsigned int param_4)

{
    unsigned char *puVar1;
    unsigned int uVar2;
    int iVar3;

    if (*(unsigned short *)(param_1 + 0x14) == param_2) {
        puVar1 = func_0200847c();
        uVar2 = func_020120f0((int)puVar1,0);
        if (uVar2 != 0) {
            puVar1 = func_0200847c();
            iVar3 = func_02012100((int)puVar1,0);
            if (iVar3 != 0) {
                return;
            }
        }
    }
    *(short *)(param_1 + 0x14) = (short)param_2;
    *(short *)(param_1 + 0x16) = (short)param_3;
    puVar1 = func_0200847c();
    func_0202a044(puVar1,(unsigned char *)param_2,param_3,param_4);
    return;
}
