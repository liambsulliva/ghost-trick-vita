int func_020330a4(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 12);
    value = value << 21;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
