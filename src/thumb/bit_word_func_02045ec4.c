int func_02045ec4(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 12);
    value = value << 16;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
