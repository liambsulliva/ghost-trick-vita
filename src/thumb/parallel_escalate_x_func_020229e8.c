extern unsigned short data_020c9d50[];

unsigned int func_020229e8(unsigned int mask)
{
    unsigned int result;
    int i;

    result = 0;
    for (i = 0; i < 0x20; i++) {
        if (mask & (1u << i)) {
            result |= data_020c9d50[i + 14];
        }
    }
    return result;
}
