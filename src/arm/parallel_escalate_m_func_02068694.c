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

extern int func_0207a540(int value);

static inline void *AddU32ToPtr(void *ptr, unsigned int val)
{
    return (void *)((unsigned int)ptr + val);
}

static inline void *GetMemPtrForMBlock(struct MBlockHead *pMBlkHd)
{
    return AddU32ToPtr(pMBlkHd, sizeof(struct MBlockHead));
}

static inline void *GetMBlockEndAddr(struct MBlockHead *pMBlkHd)
{
    return (void *)(pMBlkHd->blockSize + (unsigned int)GetMemPtrForMBlock(pMBlkHd));
}

static inline unsigned int GetOffsetFromPtr(void *start, void *end)
{
    return (unsigned int)end - (unsigned int)start;
}

unsigned int func_02068694(struct HeapHead *heap, int alignment)
{
    alignment = func_0207a540(alignment);
    {
        struct ExpHeapHead *pEHHead = (struct ExpHeapHead *)((unsigned int)heap + sizeof(struct HeapHead));
        unsigned int maxSize = 0;
        unsigned int offsetMin = 0xFFFFFFFF;
        struct MBlockHead *pMBlkHd;

        for (pMBlkHd = pEHHead->mbFreeList.head; pMBlkHd; pMBlkHd = pMBlkHd->pMBHeadNext) {
            void *baseAddress = (void *)(((unsigned int)GetMemPtrForMBlock(pMBlkHd) + (alignment - 1)) & ~(alignment - 1));

            if ((unsigned int)baseAddress < (unsigned int)GetMBlockEndAddr(pMBlkHd)) {
                unsigned int blockSize = GetOffsetFromPtr(baseAddress, GetMBlockEndAddr(pMBlkHd));
                unsigned int offset = GetOffsetFromPtr(GetMemPtrForMBlock(pMBlkHd), baseAddress);

                if (maxSize < blockSize || (maxSize == blockSize && offsetMin > offset)) {
                    maxSize = blockSize;
                    offsetMin = offset;
                }
            }
        }
        return maxSize;
    }
}
