extern short data_02080dc4[];
extern signed char data_02080db1[];

short func_020054a8(unsigned int param_1, short *param_2)
{
    int iVar1;
    int base;
    int idx;
    int iVar2;

    idx = ((unsigned char *)param_2)[2];
    base = *param_2;
    iVar1 = data_02080dc4[idx];
    iVar2 = iVar1 >> 3;
    if ((param_1 & 4) != 0) {
        iVar2 = iVar2 + iVar1;
    }
    if ((param_1 & 2) != 0) {
        iVar2 = iVar2 + (iVar1 >> 1);
    }
    if ((param_1 & 1) != 0) {
        iVar2 = iVar2 + (iVar1 >> 2);
    }
    if ((param_1 & 8) != 0) {
        base = base - iVar2;
        if (base < -0x8000) {
            base = -0x8000;
        }
    } else {
        base = base + iVar2;
        if (base > 0x7fff) {
            base = 0x7fff;
        }
    }
    idx = idx + data_02080db1[param_1];
    if (idx < 0) {
        idx = 0;
    } else if (idx > 0x58) {
        idx = 0x58;
    }
    *param_2 = (short)base;
    ((unsigned char *)param_2)[2] = (unsigned char)idx;
    return (short)base;
}
