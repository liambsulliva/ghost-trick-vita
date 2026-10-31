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

struct MBlockList {
    void *head;
    void *tail;
};

struct ExpHeapHead {
    struct MBlockList mbFreeList;
    struct MBlockList mbUsedList;
    unsigned short groupID;
    unsigned short feature;
};

struct MemRegion {
    void *start;
    void *end;
};

extern void func_02068054(struct HeapHead *heap, unsigned int signature, void *heapStart, void *heapEnd, unsigned short optFlag);
extern void *func_02068184(struct MemRegion *region, unsigned short signature);

struct HeapHead *func_020681b0(void *startAddress, void *endAddress, unsigned short optFlag)
{
    struct HeapHead *pHeapHd;
    struct ExpHeapHead *pExpHeapHd;

    pHeapHd = (struct HeapHead *)startAddress;
    pExpHeapHd = (struct ExpHeapHead *)((unsigned int)pHeapHd + sizeof(struct HeapHead));
    func_02068054(pHeapHd, 0x45585048, (void *)((unsigned int)pExpHeapHd + sizeof(struct ExpHeapHead)), endAddress, optFlag);
    pExpHeapHd->groupID = 0;
    pExpHeapHd->feature = 0;
    pExpHeapHd->feature = (unsigned short)((pExpHeapHd->feature & ~1) | 0);
    {
        void *pMBHead;
        struct MemRegion region;

        region.start = pHeapHd->heapStart;
        region.end = pHeapHd->heapEnd;
        pMBHead = func_02068184(&region, 0x4652);
        pExpHeapHd->mbFreeList.head = pMBHead;
        pExpHeapHd->mbFreeList.tail = pMBHead;
        pExpHeapHd->mbUsedList.head = 0;
        pExpHeapHd->mbUsedList.tail = 0;
    }
    return pHeapHd;
}
