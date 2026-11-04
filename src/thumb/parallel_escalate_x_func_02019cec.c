extern const unsigned short data_02084848[];

unsigned char func_02019cec(void *a, unsigned short *b)
{
    unsigned char n;
    unsigned short *p;
    unsigned short w;
    unsigned short skip;

    n = 1;
    p = b;
    for (; (w = *p) != 0xfffe; p = p + 1) {
        if (w >= 0xff00) {
            skip = data_02084848[(unsigned short)(w - 0xff00)];
            if ((w == 0xff02 || w == 0xff15) && p[skip + 1] != 0xfffe) {
                n = n + 1;
            }
            p = p + skip;
        }
    }
    return n;
}
