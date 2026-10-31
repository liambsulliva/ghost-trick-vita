int func_0206b284(unsigned int v)
{
    int r;
    unsigned int b;
    r = 0;
    b = v - 9;
    if (b > 26) {
        return r;
    }
    if (0x0400030f & (1 << b)) {
        r = 1;
    }
    return r;
}
