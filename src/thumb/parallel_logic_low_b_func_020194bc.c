void func_020194bc(unsigned char *object,unsigned int value)
{
    if (*(unsigned short *)(object + 0x2e) > value) {
        *(unsigned short *)(object + 0x2e) = value;
    }
    return;
}
