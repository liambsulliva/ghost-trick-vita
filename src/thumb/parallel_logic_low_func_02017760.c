void func_02017760(int object)
{
    unsigned char *p;
    unsigned char byte;
    unsigned char zero;

    p = (unsigned char *)object;
    p = p + 0x4e;
    byte = *p;
    if (byte != 0) {
        p = (unsigned char *)object;
        zero = 0;
        p = p + 0x4e;
        goto store;
    }
    p = (unsigned char *)object;
    p = p + 0x4f;
    byte = *p;
    if (byte == 0) {
        goto word;
    }
    p = (unsigned char *)object;
    zero = 0;
    p = p + 0x4f;
store:
    *p = zero;
word:
    *(int *)(object + 0x54) = 0;
}
