typedef struct Mtx33e6bc {
    int _00, _01, _02;
    int _10, _11, _12;
    int _20, _21, _22;
} Mtx33e6bc;

void func_0206e6bc(const Mtx33e6bc *pSrc, Mtx33e6bc *pDst, int x, int y, int z)
{
    pDst->_00 = (int)(((long long)x * pSrc->_00) >> 12);
    pDst->_01 = (int)(((long long)x * pSrc->_01) >> 12);
    pDst->_02 = (int)(((long long)x * pSrc->_02) >> 12);
    pDst->_10 = (int)(((long long)y * pSrc->_10) >> 12);
    pDst->_11 = (int)(((long long)y * pSrc->_11) >> 12);
    pDst->_12 = (int)(((long long)y * pSrc->_12) >> 12);
    pDst->_20 = (int)(((long long)z * pSrc->_20) >> 12);
    pDst->_21 = (int)(((long long)z * pSrc->_21) >> 12);
    pDst->_22 = (int)(((long long)z * pSrc->_22) >> 12);
}
