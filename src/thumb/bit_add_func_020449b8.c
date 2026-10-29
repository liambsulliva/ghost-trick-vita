int func_020449b8(unsigned char *object)
{
    unsigned int value;
    value = object[224];
    value = value << 29;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
