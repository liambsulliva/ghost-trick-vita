unsigned short * func_02068184(unsigned int *param_1,unsigned short param_2)

{
    int iVar1;
    unsigned short *puVar2;

    puVar2 = (unsigned short *)*param_1;
    iVar1 = param_1[1];
    *puVar2 = param_2;
    puVar2[1] = 0;
    *(int *)(puVar2 + 2) = iVar1 - (int)(puVar2 + 8);
    *(unsigned int *)(puVar2 + 4) = 0;
    *(unsigned int *)(puVar2 + 6) = 0;
    return puVar2;
}
