struct FlagByte {
    unsigned char before[0xc];
    unsigned char low : 2;
    unsigned char flag : 1;
    unsigned char rest : 5;
};

unsigned char func_0200b794(struct FlagByte *object)
{
    if (object->flag != 0) {
        return 1;
    }
    return 0;
}
