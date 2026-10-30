void func_020224f8(int value, unsigned char *bits, unsigned int index)
{
    unsigned int offset;
    unsigned int mask;

    offset = index >> 3;
    mask = (unsigned char)~(1 << (index & 7));
    value = bits[offset];
    value = value & mask;
    bits[offset] = value;
}
