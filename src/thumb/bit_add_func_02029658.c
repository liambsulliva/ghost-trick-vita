int func_02029658(unsigned char *object)
{
    unsigned int value;
    value = object[222];
    value = value << 27;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
