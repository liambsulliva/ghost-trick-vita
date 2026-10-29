unsigned int func_02038a84(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 16);
    return (unsigned char)((value << 16) >> 24);
}
