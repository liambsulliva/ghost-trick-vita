typedef struct MtxFx43 {
    int _00, _01, _02;
    int _10, _11, _12;
    int _20, _21, _22;
    int _30, _31, _32;
} MtxFx43;

extern void func_0206e6bc(const MtxFx43 *pSrc, MtxFx43 *pDst, int x, int y, int z);

void func_0206ea80(const MtxFx43 *pSrc, MtxFx43 *pDst, int x, int y, int z)
{
    func_0206e6bc(pSrc, pDst, x, y, z);
    pDst->_30 = pSrc->_30;
    pDst->_31 = pSrc->_31;
    pDst->_32 = pSrc->_32;
}
