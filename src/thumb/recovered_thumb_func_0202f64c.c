typedef int code();
extern int func_02009900();
extern int func_0202f678();

unsigned int func_0202f64c(int *param_1)

{
    unsigned int uVar1;
    int iVar2;

    if ((param_1 != (int *)0x0) && (iVar2 = *param_1, iVar2 != 0)) {
        uVar1 = func_02009900();
        iVar2 = func_0202f678(uVar1,iVar2);
        if (iVar2 != 0) {
            (**(code **)(*(int *)*param_1 + 0x20))((int *)*param_1,0);
            *param_1 = 0;
        }
    }
    return 1;
}
