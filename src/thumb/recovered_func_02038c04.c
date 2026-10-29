extern int func_020379f8();
extern int func_0203971c();
extern int func_02039ea0();

void func_02038c04(unsigned int *param_1)

{
    int iVar1;

    iVar1 = func_02039ea0(*param_1);
    if (iVar1 != 0) {
        func_0203971c(iVar1,0);
        func_020379f8(iVar1);
    }
    return;
}
