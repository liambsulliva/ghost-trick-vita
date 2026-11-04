struct Board;

void func_0201ce3c(struct Board *board, unsigned char *dst, unsigned int pitch, unsigned char color)
{
    int value;

    value = color | (color << 4);
    value = (color >> 4) | value;
    dst[0] = value;
    dst[pitch] = value;
}
