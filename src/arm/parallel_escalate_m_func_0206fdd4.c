void *func_0206fdd4(void)
{
    int bgMode;
    unsigned int bg;
    unsigned int offset;

    bgMode = (int)(*(volatile unsigned int *)0x04001000 & 7);
    bg = *(volatile unsigned short *)0x0400100c;
    offset = (bg & 0x1f00) >> 8;
    switch (bgMode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *)(0x6200000 + offset * 0x800);
        break;
    case 5:
        if (bg & 0x80) {
            return (void *)(0x6200000 + offset * 0x4000);
        } else {
            return (void *)(0x6200000 + offset * 0x800);
        }
        break;
    case 6:
        return 0;
        break;
    default:
        return 0;
    }
}
