unsigned int func_02016a64(char *object)
{
    unsigned int value;
    value = *(unsigned short *)(object + 32);
    return (value << 24) >> 24;
}
