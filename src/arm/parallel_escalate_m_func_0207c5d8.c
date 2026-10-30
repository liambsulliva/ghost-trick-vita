extern unsigned int func_0207c5cc(void);

int func_0207c5d8(unsigned int param_1, unsigned char *buf, unsigned int *len)
{
    unsigned int i;
    unsigned int count;
    unsigned int c;

    count = *len;
    i = 0;
    if (i < count) {
        do {
            c = func_0207c5cc();
            buf[i] = (unsigned char)c;
            if ((c & 0xff) == 13 || (c & 0xff) == 10) {
                *len = i + 1;
                break;
            }
            i = i + 1;
        } while (i < count);
    }
    return 0;
}
