unsigned int func_ov004_020d4d7c(unsigned int a, unsigned int b)
{
    unsigned int *s;

    s = (unsigned int *)0x020e4820;
    s[4] = a;
    s[3] = b & 0xfffffffc;
    s[1] = 0;
    s[0] = 0;
    s[8] = 0;
    return 0;
}
