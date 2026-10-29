int func_02033100(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 19;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
