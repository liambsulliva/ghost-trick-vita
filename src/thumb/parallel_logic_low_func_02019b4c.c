extern int func_0201e02c(int object,unsigned int a,unsigned int b);

int func_02019b4c(int object)
{
    unsigned int byte;

    byte = *(unsigned short *)(*(int *)(object + 0x14) + 2);
    byte = (byte << 24) >> 24;
    func_0201e02c(*(int *)(object + 0x6c),*(unsigned char *)(object + 0x70),byte);
    return 0;
}
