struct Bit0Byte {
    unsigned char low : 1;
    unsigned char rest : 7;
};
unsigned int func_0202f24c(unsigned char *object)
{
    struct Bit0Byte *flags;
    flags = (struct Bit0Byte *)(object + 0xaa8);
    return flags->low;
}
