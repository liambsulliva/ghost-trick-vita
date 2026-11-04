unsigned short func_0200d354(void *self, unsigned short color)
{
    int g = color & 0x3e0;
    unsigned short b = ((color & 0x7c00) >> 10) << 3;
    unsigned short r = (color & 0x1f) << 3;
    unsigned int sum;
    unsigned int v;
    unsigned short gray;
    g = (unsigned short)((g >> 5) << 3);
    sum = b * 117 + (r * 306 + g * 601);
    v = (unsigned short)((sum >> 10) >> 3);
    if (v > 31) {
        v = 31;
    }
    gray = v;
    return gray << 5;
}
