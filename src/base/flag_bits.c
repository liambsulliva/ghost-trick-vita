struct FlagByte {
    unsigned char before[0x1c];
    unsigned char low : 2;
};

int func_02008370(struct FlagByte *flags)
{
    return flags->low;
}
