int func_020356f0(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 12);
    value = value << 30;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
