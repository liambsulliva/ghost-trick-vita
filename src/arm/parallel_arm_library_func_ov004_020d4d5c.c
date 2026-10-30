unsigned int func_ov004_020d4d5c(unsigned int a, unsigned int b)
{
    unsigned int *s;

    s = (unsigned int *)0x020e4820;
    s[6] = a;
    s[5] = b & 0xfffffffc;
    s[2] = 0;
    return 0;
}
