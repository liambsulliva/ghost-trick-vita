extern unsigned short data_0209e30c[];

int func_0200c44c(unsigned int *size)
{
    unsigned int v;
    int shape;
    unsigned int big;

    v = *size;
    shape = (int)(v & 0xc000) >> 14;
    big = (v & 0xc0000000) >> 30;
    return *(unsigned short *)((char *)data_0209e30c + shape * 8 + big * 2);
}
