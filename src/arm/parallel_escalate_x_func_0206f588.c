typedef struct {
    int _[4];
} MtxFx22;

void func_0206f588(unsigned int ptr, const MtxFx22 *mtx, int a, int b, int c, int d)
{
    int temp0, temp1, temp2, temp3;

    *((unsigned int *)ptr + 0) = (unsigned int)((unsigned short)(short)(mtx->_[0] >> 4) | (unsigned short)(short)(mtx->_[1] >> 4) << 0x10);
    *((unsigned int *)ptr + 1) = (unsigned int)((unsigned short)(short)(mtx->_[2] >> 4) | (unsigned short)(short)(mtx->_[3] >> 4) << 0x10);
    temp0 = c - a;
    temp1 = d - b;
    temp2 = mtx->_[0] * temp0 + mtx->_[1] * temp1 + (a << 0xC);
    temp3 = mtx->_[2] * temp0 + mtx->_[3] * temp1 + (b << 0xC);
    *((unsigned int *)ptr + 2) = (unsigned int)(temp2 >> 4);
    *((unsigned int *)ptr + 3) = (unsigned int)(temp3 >> 4);
}
