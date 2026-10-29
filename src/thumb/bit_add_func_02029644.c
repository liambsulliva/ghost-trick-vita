int func_02029644(unsigned char *object)
{
    unsigned int value;
    value = object[222];
    value = value << 28;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
