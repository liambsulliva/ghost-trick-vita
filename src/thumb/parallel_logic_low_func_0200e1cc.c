void func_0200e1cc(int param_1, unsigned char *param_2, unsigned char *param_3, unsigned int param_4)
{
    unsigned char bVar1;
    unsigned int uVar3;

    if ((param_2 != 0) && (param_3 != 0)) {
        for (uVar3 = 0; uVar3 < param_4; uVar3 = uVar3 + 1) {
            bVar1 = *param_2;
            *param_3 = (unsigned char)(bVar1 & 0xc0) >> 2 | (unsigned char)(bVar1 & 0x30) >> 4;
            param_3[1] = (unsigned char)(bVar1 & 0xc) << 2 | (unsigned char)(bVar1 & 3);
            param_3 = param_3 + 2;
            param_2 = param_2 + 1;
        }
    }
    return;
}
