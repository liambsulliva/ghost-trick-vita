extern int func_02037130();
extern int func_020379f8();
extern int func_02038384();
extern int func_020396ac();
extern int func_0203971c();
extern int func_02039ea0();

void func_0203833c(unsigned int *param_1)

{
    unsigned char uVar1;
    int iVar2;
    int iVar3;

    iVar2 = func_02039ea0(*param_1);
    if (iVar2 != 0) {
        func_0203971c(iVar2,1);
        func_020379f8(iVar2);
        uVar1 = func_02038384(iVar2);
        iVar3 = func_02037130(uVar1);
        if (*(short *)(param_1 + 9) == iVar3) {
            func_020396ac(iVar2,1);
            return;
        }
        func_020396ac(iVar2,0);
    }
    return;
}
