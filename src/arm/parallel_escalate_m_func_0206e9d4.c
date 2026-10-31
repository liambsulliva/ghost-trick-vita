typedef struct {
    int _00, _01, _02;
    int _10, _11, _12;
    int _20, _21, _22;
    int _30, _31, _32;
} MtxFx43;

extern void func_02072f8c(const void *src, void *dst);

void func_0206e9d4(const MtxFx43 *pSrc, MtxFx43 *pDst, int x, int y, int z)
{
    if (pSrc != pDst) {
        func_02072f8c(pSrc, pDst);
    }
    {
        long long xx = x;
        long long yy = y;
        long long zz = z;

        pDst->_30 = pSrc->_30 + (int)((xx * pSrc->_00 + yy * pSrc->_10 + zz * pSrc->_20) >> 12);
        pDst->_31 = pSrc->_31 + (int)((xx * pSrc->_01 + yy * pSrc->_11 + zz * pSrc->_21) >> 12);
        pDst->_32 = pSrc->_32 + (int)((xx * pSrc->_02 + yy * pSrc->_12 + zz * pSrc->_22) >> 12);
    }
}
