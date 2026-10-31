typedef struct NNSiFndExpHeapMBlockHead NNSiFndExpHeapMBlockHead;

struct NNSiFndExpHeapMBlockHead
{
    unsigned short signature;
    unsigned short attribute;
    unsigned int blockSize;
    NNSiFndExpHeapMBlockHead *pMBHeadPrev;
    NNSiFndExpHeapMBlockHead *pMBHeadNext;
};

typedef struct
{
    NNSiFndExpHeapMBlockHead *head;
    NNSiFndExpHeapMBlockHead *tail;
} NNSiFndExpMBlockList;

typedef struct
{
    NNSiFndExpMBlockList mbFreeList;
    NNSiFndExpMBlockList mbUsedList;
    unsigned short groupID;
    unsigned short feature;
} NNSiFndExpHeapHead;

typedef struct
{
    void *start;
    void *end;
} NNSiMemRegion;

extern NNSiFndExpHeapMBlockHead *func_0206812c(NNSiFndExpMBlockList *list, NNSiFndExpHeapMBlockHead *block);
extern NNSiFndExpHeapMBlockHead *func_02068154(NNSiFndExpMBlockList *list, NNSiFndExpHeapMBlockHead *target, NNSiFndExpHeapMBlockHead *prev);
extern NNSiFndExpHeapMBlockHead *func_02068184(const NNSiMemRegion *region, unsigned short signature);

static inline void *AddU32ToPtr(void *ptr, unsigned int val)
{
    return (void *)((unsigned int)ptr + val);
}

static inline unsigned int GetOffsetFromPtr(const void *start, const void *end)
{
    return (unsigned int)end - (unsigned int)start;
}

static inline void *GetMemPtrForMBlock_(NNSiFndExpHeapMBlockHead *pMBlkHd)
{
    return AddU32ToPtr(pMBlkHd, sizeof(NNSiFndExpHeapMBlockHead));
}

static inline void *GetMBlockEndAddr_(NNSiFndExpHeapMBlockHead *pMBHead)
{
    return (void *)(pMBHead->blockSize + (unsigned int)GetMemPtrForMBlock_(pMBHead));
}

int func_020684f0(NNSiFndExpHeapHead *pEHHead, const NNSiMemRegion *pRegion)
{
    NNSiFndExpHeapMBlockHead *pBlkPrFree = 0;
    NNSiMemRegion freeRgn = *pRegion;

    {
        NNSiFndExpHeapMBlockHead *pBlk;

        for (pBlk = pEHHead->mbFreeList.head; pBlk; pBlk = pBlk->pMBHeadNext) {
            if ((void *)pBlk < pRegion->start) {
                pBlkPrFree = pBlk;
                continue;
            }
            if ((void *)pBlk == pRegion->end) {
                freeRgn.end = GetMBlockEndAddr_(pBlk);
                func_0206812c(&pEHHead->mbFreeList, pBlk);
            }
            break;
        }
    }

    if (pBlkPrFree && GetMBlockEndAddr_(pBlkPrFree) == pRegion->start) {
        freeRgn.start = pBlkPrFree;
        pBlkPrFree = func_0206812c(&pEHHead->mbFreeList, pBlkPrFree);
    }

    if (GetOffsetFromPtr(freeRgn.start, freeRgn.end) < sizeof(NNSiFndExpHeapMBlockHead)) {
        return 0;
    }

    func_02068154(&pEHHead->mbFreeList, func_02068184(&freeRgn, 0x4652), pBlkPrFree);
    return 1;
}
