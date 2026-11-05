#pragma opt_propagation off
unsigned short func_0200d1f4(void *self, unsigned short color)
{
    unsigned int r;
    unsigned int b;
    unsigned int sum;
    unsigned int red;
    unsigned int green;
    unsigned int blue;

    r = (color & 0x1f) << 3;
    b = ((color & 0x7c00) >> 10) << 3;
    sum = b * 117 + (r * 306 + (unsigned int)(((color & 0x3e0) >> 5) << 3) * 601);
    red = (sum * 9 / 10) >> 10;
    green = (sum * 7 / 10) >> 10;
    blue = (sum * 4 / 10) >> 10;
    if (red > 255) {
        red = 255;
    }
    if (green > 255) {
        green = 255;
    }
    if (blue > 255) {
        blue = 255;
    }
    return (red >> 3) | ((green >> 3) << 5) | ((blue >> 3) << 10);
}
