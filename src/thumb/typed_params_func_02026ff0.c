extern int func_02008828();
extern int func_020266dc();
extern int func_020266f4();
extern int func_0202680c();

void func_02026ff0(unsigned int param_1,unsigned char *param_2)

{
    int iVar1;
    unsigned int uVar2;
    unsigned char *puVar3;
    unsigned int uVar4;

    if ((param_2 != (unsigned char *)0x0) && (iVar1 = func_02008828(param_2,((unsigned char *)0x020cf898u)), iVar1 != 0))
    {
        uVar2 = func_020266f4((int)param_2);
        for (uVar4 = 0; uVar4 < uVar2; uVar4 = uVar4 + 1) {
            puVar3 = (unsigned char *)func_020266dc((int)param_2,uVar4);
            func_0202680c(puVar3,0);
        }
    }
    return;
}
