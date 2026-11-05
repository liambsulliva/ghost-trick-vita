#pragma opt_propagation off
unsigned short func_0200d140(void *self, unsigned short color)
{
    unsigned short r;
    unsigned short b;
    unsigned int sum;
    unsigned short gray;
    unsigned int rr; unsigned int gg; unsigned int bb;
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    r = (color & 0x1f) << 3;
    b = ((color & 0x7c00) >> 10) << 3;
    sum = b * 117 + (r * 306 + (unsigned short)(((color & 0x3e0) >> 5) << 3) * 601);
    gray = sum >> 10;
    rr = gray;
    gg = gray;
    bb = gray;
    red = rr <= 255 ? rr : 255;
    green = gg > 255 ? 255 : gg;
    blue = bb > 255 ? 255 : bb;
    return (red >> 3) | ((green >> 3) << 5) | ((blue >> 3) << 10);
}
