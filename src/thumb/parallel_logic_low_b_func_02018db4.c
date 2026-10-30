unsigned int func_02018db4(unsigned char *object)
{
    unsigned int value;
    value = *(unsigned int *)(object + 0xc);
    if ((value >> 31) != 0) {
        return 1;
    }
    return 0;
}
