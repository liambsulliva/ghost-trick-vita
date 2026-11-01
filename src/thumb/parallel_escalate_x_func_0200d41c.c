unsigned short func_0200d41c(void *self, unsigned short color)
{
    unsigned short r = (color & 0x1f) << 3;
    unsigned short g = ((color & 0x3e0) >> 5) << 3;
    unsigned short b = ((color & 0x7c00) >> 10) << 3;
    unsigned int sum = r * 306 + g * 601 + b * 117;
    unsigned int v = (unsigned short)((sum >> 10) >> 3);
    unsigned short gray;
    if (v > 31) {
        v = 31;
    }
    gray = v;
    return gray << 10;
}
