int func_020322d4(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 18;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
