int func_0203435c(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 16;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
