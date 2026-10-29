int func_0200f320(char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 8);
    value = value << 26;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
