extern long long func_0207b674(long long a, long long b);
extern void func_0200c2b4(int *dst, const int *src);

void func_0200c220(const int *m, const int *v, int *dst)
{
    int tmp[2];
    int *out;

    out = dst;
    if (dst == v) {
        out = tmp;
    }
    out[0] = (int)((func_0207b674((long long)m[2], (long long)v[1]) + 0x1000 + func_0207b674((long long)m[0], (long long)v[0])) >> 12);
    out[1] = (int)((func_0207b674((long long)m[3], (long long)v[1]) + 0x1000 + func_0207b674((long long)m[1], (long long)v[0])) >> 12);
    if (out == tmp) {
        func_0200c2b4(dst, tmp);
    }
}
