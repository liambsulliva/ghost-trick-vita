extern int func_0202f64c();
extern int func_02031270();
extern int func_0203a0c0();

void func_02039e1c(int *param_1,int param_2)

{
    int iVar1;
    int *piVar2;

    piVar2 = (int *)*param_1;
    if ((piVar2 != (int *)0x0) &&
          (iVar1 = func_0203a0c0(param_1,(unsigned char *)piVar2[2],param_2), iVar1 != 0)) {
        iVar1 = *piVar2;
        func_0202f64c((unsigned char *)(*param_1 + 8));
        if (*param_1 != 0) {
            func_02031270(*param_1);
            *param_1 = 0;
        }
        *param_1 = iVar1;
        *(unsigned int *)(iVar1 + 4) = 0;
    }
    return;
}
