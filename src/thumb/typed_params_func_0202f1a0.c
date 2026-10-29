extern int func_0202f384();

unsigned int * func_0202f1a0(unsigned int *param_1)

{
    int iVar1;

    iVar1 = ((int)0x00004024u);
    *param_1 = 0;
    *(unsigned int *)((int)param_1 + iVar1) = 0;
    *(unsigned char *)((int)param_1 + iVar1 + 4) = 0;
    func_0202f384((int)param_1);
    return param_1;
}
