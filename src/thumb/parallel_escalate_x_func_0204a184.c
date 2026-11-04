void func_0204a184(void *p, int *pos, int *out, int *range, int *start, int *from, int *to)
{
    int a;
    int b;
    int c;
    int s;

    a = *pos;
    b = *out;
    s = a + b;
    c = *range;
    *pos = s;
    c = c + *start * 2;
    if (s > c) {
        *pos = c;
        *out = *from;
    }
    if (*pos < 0) {
        *pos = 0;
        *out = *to;
    }
}
