int func_020328c4(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 25;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
