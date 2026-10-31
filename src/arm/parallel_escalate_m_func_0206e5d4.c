typedef int fx32;
typedef long long fx64;

typedef struct {
    fx32 _00, _01, _10, _11;
} MtxFx22;

void func_0206e5d4(const MtxFx22 *a, const MtxFx22 *b, MtxFx22 *ab)
{
    MtxFx22 tmp;
    MtxFx22 *p;
    fx32 x, y;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }

    x = a->_00;
    y = a->_01;

    p->_00 = (fx32)(((fx64)x * b->_00 + (fx64)y * b->_10) >> 12);
    p->_01 = (fx32)(((fx64)x * b->_01 + (fx64)y * b->_11) >> 12);

    x = a->_10;
    y = a->_11;

    p->_10 = (fx32)(((fx64)x * b->_00 + (fx64)y * b->_10) >> 12);
    p->_11 = (fx32)(((fx64)x * b->_01 + (fx64)y * b->_11) >> 12);

    if (p == &tmp) {
        *ab = tmp;
    }
}
