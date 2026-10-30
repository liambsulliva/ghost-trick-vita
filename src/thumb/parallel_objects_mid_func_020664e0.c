void func_020664e0(char *p, unsigned char a, unsigned char b, unsigned char c, unsigned int d)
{
    unsigned int value = d;
    p[0x18] = a;
    p[0x19] = b;
    p[0x1a] = c;
    *(unsigned int *)(p + 0x1c) = value;
}
