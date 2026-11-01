const unsigned char *func_0207c6f8(const unsigned char *p, unsigned int *out);

const unsigned char *func_0207dff4(const unsigned char *param_1)
{
    unsigned char bVar1;
    const unsigned char *pbVar2;
    unsigned int uStack_10;

    bVar1 = *param_1;
    pbVar2 = func_0207c6f8(param_1 + 2,&uStack_10);
    if ((bVar1 & 0x40) == 0) {
        return pbVar2;
    }
    return func_0207c6f8(pbVar2,&uStack_10);
}
