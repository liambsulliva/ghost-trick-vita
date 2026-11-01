int func_0202adc0(unsigned char *p, int kind)
{
    switch (kind) {
    case 0:
        return 0xf000;
    case 1:
        if (*(short *)(p + 0x26) == 0) {
            return 0x71000;
        }
        return 0xd0000;
    case 2:
        if (*(short *)(p + 0x26) == 0) {
            return 0x80000;
        }
        return 0x21000;
    default:
        return 0;
    }
}
