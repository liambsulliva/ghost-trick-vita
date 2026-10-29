extern int func_02010b54();
extern int func_02038790();
extern int func_02038b34();

unsigned int func_02038afc(unsigned int *param_1,int param_2)

{
    unsigned int *puVar1;
    int iVar2;

    puVar1 = (unsigned int *)func_02038790(*param_1);
    if (puVar1 != (unsigned int *)0x0) {
        for (; puVar1 != (unsigned int *)0x0; puVar1 = (unsigned int *)*puVar1) {
            if (((puVar1[2] != 0) && (iVar2 = func_02038b34(), param_2 == iVar2)) &&
                  (iVar2 = func_02010b54(puVar1[2]), iVar2 != 0)) {
                return 1;
            }
        }
    }
    return 0;
}
