unsigned int func_02011320(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    return (unsigned char)((value << 8) >> 24);
}
