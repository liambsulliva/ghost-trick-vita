void func_0201e230(int object,unsigned int value)
{
    unsigned short *slot;
    unsigned int mask;

    slot = (unsigned short *)(object - 16);
    mask = 0xff;
    mask = mask << 8;
    slot[1] = slot[1] & mask;
    value = (value << 16) >> 16;
    slot[1] = slot[1] | value;
}
