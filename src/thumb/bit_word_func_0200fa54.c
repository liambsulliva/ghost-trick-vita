int func_0200fa54(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 16);
    value = value << 5;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
