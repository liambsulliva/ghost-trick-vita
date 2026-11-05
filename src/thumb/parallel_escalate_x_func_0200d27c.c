#pragma opt_propagation off
unsigned short func_0200d27c(void *self, unsigned short color)
{
    unsigned short r;
    unsigned short b;
    unsigned int sum;
    unsigned short gray;
    unsigned int v;
    int ig;

    r = (color & 0x1f) << 3;
    b = ((color & 0x7c00) >> 10) << 3;
    sum = b * 117 + (r * 306 + (unsigned short)(((color & 0x3e0) >> 5) << 3) * 601);
    gray = sum >> 13;
    v = gray;
    if (v > 31) {
        v = 31;
    }
    ig = (unsigned short)v;
    return ig;
}
