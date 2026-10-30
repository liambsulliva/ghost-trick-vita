void func_0200e214(int param_1, unsigned char *param_2, unsigned char *param_3, unsigned int param_4)
{
    unsigned int uVar2;
    unsigned char bVar1;

    if ((param_2 != 0) && (param_3 != 0)) {
        for (uVar2 = 0; uVar2 < param_4; uVar2 = uVar2 + 1) {
            bVar1 = *param_2;
            param_2 = param_2 + 1;
            *param_3 = bVar1 & 0xf;
            param_3[1] = (unsigned char)(bVar1 & 0xf0) >> 4;
            param_3 = param_3 + 2;
        }
    }
    return;
}
