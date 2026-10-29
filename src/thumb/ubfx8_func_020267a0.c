unsigned int func_020267a0(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    return (unsigned char)((value << 27) >> 31);
}
