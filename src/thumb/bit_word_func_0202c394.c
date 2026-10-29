int func_0202c394(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 12);
    value = value << 19;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
