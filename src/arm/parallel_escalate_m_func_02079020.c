int func_02079020(signed char *s)
{
    unsigned int i;
    unsigned int *w;
    unsigned int v;
    unsigned int t;

    i = 0;
    if (((unsigned int)s & 3) != 0) {
        do {
            if (s[i] == 0) {
                return (int)i;
            }
            i++;
        } while ((((unsigned int)s + i) & 3) != 0);
    }
    w = (unsigned int *)(s + i);
    for (;;) {
        v = *w;
        t = ~(((v & 0x7f7f7f7f) + 0x7f7f7f7f) | v | 0x7f7f7f7f);
        if (t != 0) {
            break;
        }
        w++;
        i += 4;
    }
    while (s[i] != 0) {
        i++;
    }
    return (int)i;
}
