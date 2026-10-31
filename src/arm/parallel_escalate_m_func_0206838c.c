struct MBlockHead {
    unsigned short signature;
    unsigned short attribute;
    unsigned int blockSize;
    struct MBlockHead *pMBHeadPrev;
    struct MBlockHead *pMBHeadNext;
};

struct MBlockList {
    struct MBlockHead *head;
    struct MBlockHead *tail;
};

struct ExpHeapHead {
    struct MBlockList mbFreeList;
    struct MBlockList mbUsedList;
    unsigned short groupID;
    unsigned short feature;
};

struct Link {
    void *prev;
    void *next;
};

struct List {
    void *head;
    void *tail;
    unsigned short count;
    unsigned short offset;
};

struct HeapHead {
    unsigned int signature;
    struct Link link;
    struct List childList;
    void *heapStart;
    void *heapEnd;
    unsigned int attribute;
};

extern void *func_02068220(struct ExpHeapHead *pEHHead, struct MBlockHead *pMBlkHd, void *mblock, unsigned int size, int direction);

static inline void *AddU32ToPtr(void *ptr, unsigned int val)
{
    return (void *)((unsigned int)ptr + val);
}

static inline void *GetMemPtrForMBlock(struct MBlockHead *pMBlkHd)
{
    return AddU32ToPtr(pMBlkHd, sizeof(struct MBlockHead));
}

static inline unsigned int GetOffsetFromPtr(void *start, void *end)
{
    return (unsigned int)end - (unsigned int)start;
}

static inline unsigned short GetAllocMode(struct ExpHeapHead *pEHHead)
{
    return (unsigned short)(pEHHead->feature & 1);
}

void *func_0206838c(struct HeapHead *pHeapHd, unsigned int size, int alignment)
{
    struct ExpHeapHead *pEHHead = (struct ExpHeapHead *)AddU32ToPtr(pHeapHd, sizeof(struct HeapHead));
    const int isFirst = GetAllocMode(pEHHead) == 0;
    struct MBlockHead *pMBlkHd = 0;
    struct MBlockHead *pMBlkHdFound = 0;
    unsigned int foundSize = 0xFFFFFFFF;
    void *foundMBlock = 0;

    {
        for (pMBlkHd = pEHHead->mbFreeList.head; pMBlkHd; pMBlkHd = pMBlkHd->pMBHeadNext) {
            void *const mblock = GetMemPtrForMBlock(pMBlkHd);
            void *const reqMBlock = (void *)(((alignment - 1) + (unsigned int)mblock) & ~(alignment - 1));
            const unsigned int offset = GetOffsetFromPtr(mblock, reqMBlock);

            if (pMBlkHd->blockSize >= size + offset && foundSize > pMBlkHd->blockSize) {
                pMBlkHdFound = pMBlkHd;
                foundSize = pMBlkHd->blockSize;
                foundMBlock = reqMBlock;

                if (isFirst || foundSize == size) {
                    break;
                }
            }
        }
    }

    if (!pMBlkHdFound) {
        return 0;
    }

    return func_02068220(pEHHead, pMBlkHdFound, foundMBlock, size, 0);
}
