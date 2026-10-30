unsigned int func_0201b4c4(unsigned char *object)
{
    unsigned int high;
    unsigned int low;
    high = *(unsigned short *)(object + 2);
    high = high << 16;
    low = *(unsigned short *)object;
    return high | low;
}
