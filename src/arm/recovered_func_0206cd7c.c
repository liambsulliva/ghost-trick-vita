extern int func_0206c8ec();
extern int func_0206ca4c();
extern int func_0206cb8c();

unsigned int func_0206cd7c(unsigned int param_1,unsigned int param_2)

{
    int iVar1;
    unsigned int uVar2;
    unsigned char auStack_38 [44];

    uVar2 = 0;
    iVar1 = func_0206c8ec(auStack_38,param_1,param_2);
    if ((iVar1 != 0) && (iVar1 = func_0206ca4c(auStack_38), iVar1 != 0)) {
        func_0206cb8c(auStack_38);
        uVar2 = 1;
    }
    return uVar2;
}
