void *func_0206fd50(void)
{
    int bgMode;
    unsigned int bg;
    unsigned int offset1;
    unsigned int block;

    bgMode = (int)(*(volatile unsigned int *)0x04000000 & 7);
    bg = *(volatile unsigned short *)0x0400000c;
    offset1 = ((*(volatile unsigned int *)0x04000000 & 0x38000000) >> 27) * 0x10000;
    block = (bg & 0x1f00) >> 8;
    switch (bgMode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *)(0x6000000 + offset1 + block * 0x800);
        break;
    case 5:
        if (bg & 0x80) {
            return (void *)(0x6000000 + block * 0x4000);
        } else {
            return (void *)(0x6000000 + offset1 + block * 0x800);
        }
        break;
    case 6:
        return (void *)0x6000000;
        break;
    default:
        return 0;
    }
}
