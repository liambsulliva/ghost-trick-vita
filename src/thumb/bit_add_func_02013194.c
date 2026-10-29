int func_02013194(unsigned char *object)
{
    unsigned int value;
    value = object[212];
    value = value << 28;
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
