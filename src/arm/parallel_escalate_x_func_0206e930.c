typedef struct {
    int x;
    int y;
    int z;
} VecFx32;

typedef struct {
    int _00, _01, _02;
    int _10, _11, _12;
    int _20, _21, _22;
} MtxFx33;

void func_0206e930(const VecFx32 *vec, const MtxFx33 *m, VecFx32 *dst)
{
    int vx;
    int vy;
    int vz;

    vx = vec->x;
    vy = vec->y;
    vz = vec->z;
    dst->x = (int)(((long long)vx * m->_00 + (long long)vy * m->_10 + (long long)vz * m->_20) >> 12);
    dst->y = (int)(((long long)vx * m->_01 + (long long)vy * m->_11 + (long long)vz * m->_21) >> 12);
    dst->z = (int)(((long long)vx * m->_02 + (long long)vy * m->_12 + (long long)vz * m->_22) >> 12);
}
