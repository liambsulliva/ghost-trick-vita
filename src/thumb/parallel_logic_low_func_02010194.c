int func_02010194(int object)
{
    unsigned int bits;
    unsigned int disp;

    bits = *(unsigned int *)(object + 0xc);
    bits = bits << 0x11;
    bits = bits >> 0x1e;
    if (bits == 1) {
        disp = 0xe44;
        return *(int *)((char *)object + disp);
    }
    if (bits == 2) {
        disp = 0xe48;
        return *(int *)((char *)object + disp);
    }
    disp = 0x39;
    disp = disp << 6;
    return *(int *)((char *)object + disp);
}
