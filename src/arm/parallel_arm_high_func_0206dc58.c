extern int func_0206d9ec(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int *param_4, int param_5);
extern int func_0206dabc(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);

void func_0206dc58(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5)
{
    unsigned int uStack_14;
    int iVar1;

    iVar1 = func_0206d9ec(param_1, param_3, param_4, &uStack_14, 0);
    if (iVar1 == 0) {
        func_0206dabc(param_1, param_2, uStack_14, param_5);
    }
}
