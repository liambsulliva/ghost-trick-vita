extern const unsigned char data_020a2664[];

unsigned short func_020772f8(int param_1)
{
    unsigned char bVar1;
    int iVar2;

    if (param_1 < -723) {
        param_1 = -723;
    }
    else if (param_1 > 0) {
        param_1 = 0;
    }
    bVar1 = data_020a2664[param_1 + 723];
    if (param_1 < -240) {
        iVar2 = 3;
    }
    else if (param_1 < -120) {
        iVar2 = 2;
    }
    else if (param_1 < -60) {
        iVar2 = 1;
    }
    else {
        iVar2 = 0;
    }
    return bVar1 | (iVar2 << 8);
}
