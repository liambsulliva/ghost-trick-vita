typedef struct Mtx43ead0 {
    int _00, _01, _02;
    int _10, _11, _12;
    int _20, _21, _22;
    int _30, _31, _32;
} Mtx43ead0;

void func_0206ead0(const Mtx43ead0 *a, const Mtx43ead0 *b, Mtx43ead0 *ab)
{
    Mtx43ead0 tmp;
    Mtx43ead0 *p;
    int x, y, z;
    int xx, yy, zz;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }

    x = a->_00;
    y = a->_01;
    z = a->_02;

    p->_00 = (int)(((long long)x * b->_00 + (long long)y * b->_10 + (long long)z * b->_20) >> 12);
    p->_01 = (int)(((long long)x * b->_01 + (long long)y * b->_11 + (long long)z * b->_21) >> 12);

    xx = b->_02;
    yy = b->_12;
    zz = b->_22;

    p->_02 = (int)(((long long)x * xx + (long long)y * yy + (long long)z * zz) >> 12);

    x = a->_10;
    y = a->_11;
    z = a->_12;

    p->_12 = (int)(((long long)x * xx + (long long)y * yy + (long long)z * zz) >> 12);
    p->_11 = (int)(((long long)x * b->_01 + (long long)y * b->_11 + (long long)z * b->_21) >> 12);

    xx = b->_00;
    yy = b->_10;
    zz = b->_20;

    p->_10 = (int)(((long long)x * xx + (long long)y * yy + (long long)z * zz) >> 12);

    x = a->_20;
    y = a->_21;
    z = a->_22;

    p->_20 = (int)(((long long)x * xx + (long long)y * yy + (long long)z * zz) >> 12);
    p->_21 = (int)(((long long)x * b->_01 + (long long)y * b->_11 + (long long)z * b->_21) >> 12);

    xx = b->_02;
    yy = b->_12;
    zz = b->_22;

    p->_22 = (int)(((long long)x * xx + (long long)y * yy + (long long)z * zz) >> 12);

    x = a->_30;
    y = a->_31;
    z = a->_32;

    p->_32 = (int)((((long long)x * xx + (long long)y * yy + (long long)z * zz) >> 12) + b->_32);
    p->_31 = (int)((((long long)x * b->_01 + (long long)y * b->_11 + (long long)z * b->_21) >> 12) + b->_31);
    p->_30 = (int)((((long long)x * b->_00 + (long long)y * b->_10 + (long long)z * b->_20) >> 12) + b->_30);

    if (p == &tmp) {
        *ab = tmp;
    }
}
