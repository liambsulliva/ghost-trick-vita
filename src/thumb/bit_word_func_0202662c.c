int func_0202662c(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 16);
    value = value << 9;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
