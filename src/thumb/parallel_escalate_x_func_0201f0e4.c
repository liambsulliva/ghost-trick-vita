int func_0201f0e4(int a, int b, int c, int d, int e, unsigned int f)
{
    int v;
    int s;
    int r;

    if (d >= 0) {
        v = d * e;
    } else {
        v = -(-d * e);
    }
    s = v + c;
    if (s >= 0) {
        r = (s * e) >> 1;
    } else {
        r = -((-s * e) >> 1);
    }
    return r + a;
}
