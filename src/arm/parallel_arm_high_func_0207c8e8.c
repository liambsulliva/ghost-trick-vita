unsigned char func_0207c8e8(int *s)
{
    int v;
    if (s[2] != 0) {
        v = *(unsigned char *)s[2] & 31;
    } else {
        v = 0;
    }
    return v;
}
