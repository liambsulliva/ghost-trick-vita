typedef struct PrintBuf {
    unsigned int len;
    char *cur;
    char *base;
} PrintBuf;

void func_020792e8(PrintBuf *p, const signed char *s, int n)
{
    unsigned int i;
    unsigned int lim;

    if (n <= 0) {
        return;
    }
    lim = p->len;
    i = 0;
    if (lim > n) {
        lim = n;
    }
    for (; i < lim; i++) {
        p->cur[i] = s[i];
    }
    p->len = p->len - lim;
    p->cur = p->cur + n;
}
