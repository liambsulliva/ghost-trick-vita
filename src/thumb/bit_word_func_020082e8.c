int func_020082e8(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 12);
    value = value << 3;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
