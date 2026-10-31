extern const short data_0209e4b8[];

unsigned int func_0206f2a4(int x)
{
    int left;
    int right;
    int mid;
    short xx;
    short sVar2;
    int iVar1;

    if (x >= 0) {
        left = 0;
        right = 0x400;
    } else {
        left = 0x400;
        right = 0x800;
    }
    xx = (short)x;
    while (left <= right) {
        mid = (left + right) / 2;
        sVar2 = *(const short *)((const char *)data_0209e4b8 + mid * 4 + 2);
        if (xx == sVar2) {
            break;
        }
        if (sVar2 < xx) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    iVar1 = (mid * 2 + 1) * 0x10000;
    return ((iVar1 + ((unsigned int)(iVar1 >> 12) >> 19)) << 3) >> 16;
}
