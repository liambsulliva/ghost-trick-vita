int func_02013a14(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 23;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
