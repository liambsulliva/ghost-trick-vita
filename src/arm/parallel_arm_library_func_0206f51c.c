void func_0206f51c(unsigned short *dest, int value)
{
    if (value == 0) {
        *dest = 0;
        return;
    }
    if (value > 0) {
        *dest = value | 0x4000;
    }
    else {
        *dest = -value | 0x8000;
    }
}
