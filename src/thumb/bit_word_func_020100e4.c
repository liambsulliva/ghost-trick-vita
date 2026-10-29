int func_020100e4(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 20);
    value = value << 30;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
