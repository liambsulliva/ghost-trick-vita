unsigned int func_02031b3c(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 12);
    return (value << 10) >> 29;
}
