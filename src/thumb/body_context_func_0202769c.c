extern int func_02008828();
extern int func_020266dc();
extern int func_020266f4();
extern int func_020276ec();

void func_0202769c(unsigned char *param_1,unsigned int param_2,int param_3)

{
    int iVar1;
    unsigned int uVar2;
    unsigned char *puVar3;
    unsigned int uVar4;

    iVar1 = func_02008828(param_1,((unsigned char *)0x020cf87cu));
    if (iVar1 != 0) {
        uVar2 = func_020266f4((int)param_1);
        if ((int)param_2 < 0) {
            for (uVar4 = 0; uVar4 < uVar2; uVar4 = uVar4 + 1) {
                puVar3 = (unsigned char *)func_020266dc((int)param_1,uVar4);
                func_020276ec(puVar3,param_3);
            }
            return;
        }
        if (uVar2 > param_2) {
            puVar3 = (unsigned char *)func_020266dc((int)param_1,param_2);
            func_020276ec(puVar3,param_3);
        }
    }
    return;
}
