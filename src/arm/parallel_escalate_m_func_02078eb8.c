extern int func_02079020(const char *s);

int func_02078eb8(char *destp, const char *srcp, int siz)
{
    unsigned int align;
    int i;
    const signed char *s;
    unsigned int w;

    align = (unsigned int)destp & 3;
    s = (const signed char *)srcp;
    i = 0;
    if ((align ^ ((unsigned int)srcp & 3)) == 0) {
        if (align != 0) {
            for (; i + align < 4 && i < siz - 1; i++) {
                destp[i] = *s;
                if (*s == 0) {
                    goto end;
                }
                s++;
            }
        }
        for (; i < siz - 4; i += 4) {
            w = *(const unsigned int *)(srcp + i);
            if (~(((w & 0x7f7f7f7f) + 0x7f7f7f7f) | w | 0x7f7f7f7f)) {
                break;
            }
            *(unsigned int *)(destp + i) = w;
        }
        s = (const signed char *)(srcp + i);
    }
    for (; i < siz - 1; i++) {
        destp[i] = *s;
        if (*s == 0) {
            goto end;
        }
        s++;
    }
end:
    if (i >= siz - 1 && siz > 0) {
        destp[i] = 0;
    }
    if (*s != 0) {
        i += func_02079020((const char *)s);
    }
    return i;
}
