extern int func_02052e9c();

unsigned int * func_020535d4(unsigned int *param_1)

{
    int iVar1;

    func_02052e9c();
    *param_1 = ((unsigned int)0x020ac988u);
    iVar1 = ((unsigned int)0x00000155u);
    *(unsigned char *)((int)param_1 + ((unsigned int)0x00000155u)) = 0;
    *(unsigned char *)((int)param_1 + iVar1 + -1) = 1;
    return param_1;
}
