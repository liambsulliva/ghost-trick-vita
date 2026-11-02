extern unsigned short data_0209e324[];

int func_0200c428(unsigned int *size)
{
    unsigned int v;
    int shape;
    unsigned int big;

    v = *size;
    shape = (int)(v & 0xc000) >> 14;
    big = (v & 0xc0000000) >> 30;
    return *(unsigned short *)((char *)data_0209e324 + shape * 8 + big * 2);
}
