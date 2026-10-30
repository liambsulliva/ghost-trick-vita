int func_0206bc74(signed char *dst, int dstlen, const signed char *src, int srclen, int *failure)
{
    int n = (dstlen - 1 < srclen) ? (dstlen - 1) : srclen;
    int i;
    for (i = 0; (i < n) && src[i]; ++i) {
        dst[i] = src[i];
    }
    if ((i < srclen) && src[i]) {
        *failure = 1;
    }
    dst[i] = 0;
    return i;
}
