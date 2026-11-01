unsigned short func_0200d46c(void *self, unsigned short color, int factor)
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    if (factor == 0) {
        return 0;
    }
    if (factor == 0x100) {
        return color;
    }
    r = (unsigned short)(((color & 0x1f) * factor) >> 8);
    g = (unsigned short)((((color & 0x3e0) >> 5) * factor) >> 8);
    b = (unsigned short)((((color & 0x7c00) >> 10) * factor) >> 8);
    if (r > 31) {
        r = 31;
    }
    red = r;
    if (g > 31) {
        g = 31;
    }
    green = g;
    if (b > 31) {
        b = 31;
    }
    blue = b;
    return red | (green << 5) | (blue << 10);
}
