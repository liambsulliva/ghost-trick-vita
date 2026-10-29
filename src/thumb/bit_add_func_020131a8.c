int func_020131a8(unsigned char *object)
{
    unsigned int value;
    value = object[212];
    value = value << 27;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
