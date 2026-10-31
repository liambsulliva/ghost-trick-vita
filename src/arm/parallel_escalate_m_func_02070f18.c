typedef struct {
    unsigned int unk0;
    unsigned int texBlk1;
    unsigned int plttBlk;
    unsigned int texPltt;
    unsigned int unk10;
    unsigned int tex;
    unsigned int texBlk2;
    unsigned int szTexBlk1;
} LoadState;

extern LoadState data_020d0e64;
extern unsigned int data_020b1748;

extern void func_02072974(unsigned int dmaNo, const void *src, void *dest, unsigned int size, int wait);
extern void func_02072bc8(unsigned int dmaNo, const void *src, void *dest, unsigned int size, void *callback, void *arg, int wait);
extern void func_02072ed8(const void *src, void *dest, unsigned int size);

static inline void dma_copy32(unsigned int dmaNo, const void *src, void *dest, unsigned int size)
{
    if (dmaNo != (unsigned int)~0 && size > 48) {
        func_02072974(dmaNo, src, dest, size, 1);
    } else {
        func_02072ed8(src, dest, size);
    }
}

static inline void dma_copy32_async(unsigned int dmaNo, const void *src, void *dest, unsigned int size, void *callback, void *arg)
{
    if (dmaNo != (unsigned int)~0) {
        func_02072bc8(dmaNo, src, dest, size, callback, arg, 1);
    } else {
        func_02072ed8(src, dest, size);
    }
}

void func_02070f18(const void *pSrc, unsigned int destSlotAddr, unsigned int szByte)
{
    void *pLCDC;

    if (0 == data_020d0e64.texBlk2) {
        pLCDC = (void *)(data_020d0e64.texBlk1 + destSlotAddr);
    } else {
        if (destSlotAddr + szByte < data_020d0e64.szTexBlk1) {
            pLCDC = (void *)(data_020d0e64.texBlk1 + destSlotAddr);
        } else if (destSlotAddr >= data_020d0e64.szTexBlk1) {
            pLCDC = (void *)(data_020d0e64.texBlk2 + destSlotAddr - data_020d0e64.szTexBlk1);
        } else {
            void *pLCDC2 = (void *)data_020d0e64.texBlk2;
            unsigned int sz = data_020d0e64.szTexBlk1 - destSlotAddr;
            pLCDC = (void *)(data_020d0e64.texBlk1 + destSlotAddr);
            dma_copy32(data_020b1748, pSrc, pLCDC, sz);
            dma_copy32_async(data_020b1748, (const void *)((const unsigned char *)pSrc + sz), pLCDC2, szByte - sz, 0, 0);
            return;
        }
    }
    dma_copy32_async(data_020b1748, pSrc, pLCDC, szByte, 0, 0);
}
