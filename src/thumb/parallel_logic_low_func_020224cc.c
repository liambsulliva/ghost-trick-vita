int func_020224cc(int unused,unsigned char *bits,unsigned int index)
{
    int bit;

    bit = bits[index >> 3];
    bit = bit >> (index & 7);
    if ((bit & 1) != 0) {
        return 1;
    }
    return 0;
}
