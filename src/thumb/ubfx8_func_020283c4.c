unsigned int func_020283c4(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 20);
    return (unsigned char)((value << 28) >> 30);
}
