typedef struct Mtx0206ed14 {
    int m[4][4];
} Mtx0206ed14;

extern void func_0206efd4(int denom);
extern long long func_0206ef7c(void);

static inline void inv_start(int denom)
{
    *(unsigned long long *)0x04000290 = (unsigned long long)0x1000 << 32;
    *(unsigned long long *)0x04000298 = (unsigned int)denom;
}

static inline int mul_round(int v32, long long v64)
{
    long long tmp = v64 * v32 + 0x80000000LL;
    return (int)(tmp >> 32);
}

void func_0206ed14(int t, int b, int l, int r, int n, int f, int scale, Mtx0206ed14 *mtx)
{
    long long inv1;
    long long inv2;
    long long inv3;

    func_0206efd4(r - l);
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[0][3] = 0;
    mtx->m[1][0] = 0;
    mtx->m[1][2] = 0;
    mtx->m[1][3] = 0;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][3] = 0;
    mtx->m[3][3] = scale;

    inv1 = func_0206ef7c();
    inv_start(t - b);
    if (scale != 0x1000) {
        inv1 = inv1 * scale / 0x1000;
    }
    mtx->m[0][0] = mul_round(0x2000, inv1);

    inv2 = func_0206ef7c();
    inv_start(n - f);
    if (scale != 0x1000) {
        inv2 = inv2 * scale / 0x1000;
    }
    mtx->m[1][1] = mul_round(0x2000, inv2);

    inv3 = func_0206ef7c();
    if (scale != 0x1000) {
        inv3 = inv3 * scale / 0x1000;
    }
    mtx->m[2][2] = mul_round(0x2000, inv3);
    mtx->m[3][0] = mul_round(-(r + l), inv1);
    mtx->m[3][1] = mul_round(-(t + b), inv2);
    mtx->m[3][2] = mul_round(f + n, inv3);
}
