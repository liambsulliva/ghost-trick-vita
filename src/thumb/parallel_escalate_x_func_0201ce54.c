struct Board;

void func_0201ce54(struct Board *board, unsigned char *dst, unsigned int pitch, unsigned char color)
{
    dst[0] = color;
    dst[1] = color;
    dst[pitch] = color;
    ((unsigned char *)((int)dst + pitch))[1] = color;
}
