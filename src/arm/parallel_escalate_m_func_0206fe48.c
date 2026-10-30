int func_0206fe48(void)
{
    unsigned int mode;
    unsigned int bg;
    int base;
    unsigned int tile;

    mode = *(volatile unsigned int *)0x04000000 & 7;
    bg = *(volatile unsigned short *)0x0400000e;
    base = ((*(volatile unsigned int *)0x04000000 & 0x38000000) >> 27) * 0x10000;
    tile = (bg & 0x1f00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return base + 0x6000000 + tile * 0x800;
    case 3:
    case 4:
    case 5:
        if ((bg & 0x80) != 0) {
            return tile * 0x4000 + 0x6000000;
        }
        return base + 0x6000000 + tile * 0x800;
    case 6:
        return 0;
    default:
        break;
    }
    return 0;
}
