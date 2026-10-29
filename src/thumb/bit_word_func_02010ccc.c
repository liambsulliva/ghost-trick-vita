int func_02010ccc(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 12);
    value = value << 29;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
