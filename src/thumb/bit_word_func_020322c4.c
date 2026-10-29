int func_020322c4(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 17;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
