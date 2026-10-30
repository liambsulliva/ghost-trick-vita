extern const unsigned short data_02084848[];
extern unsigned int func_02015cc4(int value);

int func_020175a4(int unused, unsigned char *p)
{
    unsigned short v;

    if (p != 0) {
        while (func_02015cc4(*(unsigned short *)p) == 0) {
            v = *(unsigned short *)p;
            if (v < 0xff00) {
                return 1;
            }
            p = p + data_02084848[(unsigned short)(v - 0xff00)] * 2 + 2;
        }
    }
    return 0;
}
