int func_020317b8(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 28;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
