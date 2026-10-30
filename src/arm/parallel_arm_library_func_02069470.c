unsigned char *func_02069470(unsigned char *s, unsigned int key)
{
    unsigned char *p;
    unsigned short i;
    unsigned short n;

    p = s + *(unsigned short *)(s + 12);
    n = *(unsigned short *)(s + 14);
    for (i = 0; i < n; i++) {
        if (*(unsigned int *)p == key) {
            return p;
        }
        p = p + *(unsigned int *)(p + 4);
    }
    return 0;
}
