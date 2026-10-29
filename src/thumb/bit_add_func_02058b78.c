int func_02058b78(unsigned char *object)
{
    unsigned int value;
    value = object[222];
    value = value << 29;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
