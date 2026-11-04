typedef struct MBlock {
    unsigned short signature;
    unsigned short attribute;
    unsigned int blockSize;
    struct MBlock *prev;
    struct MBlock *next;
} MBlock;

typedef struct MBlockList {
    MBlock *head;
    MBlock *tail;
} MBlockList;

typedef struct ExpHeap {
    MBlockList freeList;
    MBlockList usedList;
    unsigned short groupID;
    unsigned short feature;
} ExpHeap;

typedef struct Region {
    void *start;
    void *end;
} Region;

extern void func_02068100(Region *region, MBlock *block);
extern MBlock *func_0206812c(MBlockList *list, MBlock *block);
extern MBlock *func_02068154(MBlockList *list, MBlock *target, MBlock *prev);
extern MBlock *func_02068184(Region *region, unsigned short signature);
extern void func_02072ec4(unsigned int data, void *dest, unsigned int size);

#define SET_BIT_VALUE(data, st, bits, val) \
    do { \
        unsigned int maskBits = (unsigned int)((1 << (bits)) - 1); \
        unsigned int newVal = (val) & maskBits; \
        (void)(maskBits <<= st); \
        (data) &= ~maskBits; \
        (data) |= newVal << (st); \
    } while (0)

static inline void set_alloc_dir(MBlock *block, unsigned short dir)
{
    SET_BIT_VALUE(block->attribute, 15, 1, dir);
}

static inline void set_alignment(MBlock *block, unsigned short alignment)
{
    SET_BIT_VALUE(block->attribute, 8, 7, alignment);
}

static inline void set_group_id(MBlock *block, unsigned short id)
{
    SET_BIT_VALUE(block->attribute, 0, 8, id);
}

static inline unsigned short get_opt(ExpHeap *heap)
{
    return (unsigned short)(*(unsigned int *)((char *)heap - 4) & 0xff);
}

static inline void fill_alloc(ExpHeap *heap, void *address, unsigned int size)
{
    if (get_opt(heap) & 1)
    {
        func_02072ec4(0, address, size);
    }
}

static inline unsigned int get_offset(const void *start, const void *end)
{
    return (unsigned int)end - (unsigned int)start;
}

void *func_02068220(ExpHeap *heap, MBlock *freeBlock, void *mem, unsigned int size, unsigned short direction)
{
    Region freeRgnT;
    Region freeRgnB;
    MBlock *prev;

    func_02068100(&freeRgnT, freeBlock);
    freeRgnB.end = freeRgnT.end;
    freeRgnB.start = (void *)(size + (unsigned int)mem);
    freeRgnT.end = (char *)mem - sizeof(MBlock);

    prev = func_0206812c(&heap->freeList, freeBlock);

    if ((unsigned int)((char *)freeRgnT.end - (char *)freeRgnT.start) < sizeof(MBlock) + 4)
    {
        freeRgnT.end = freeRgnT.start;
    }
    else
    {
        prev = func_02068154(&heap->freeList, func_02068184(&freeRgnT, 0x4652), prev);
    }

    if ((unsigned int)((char *)freeRgnB.end - (char *)freeRgnB.start) < sizeof(MBlock) + 4)
    {
        freeRgnB.start = freeRgnB.end;
    }
    else
    {
        func_02068154(&heap->freeList, func_02068184(&freeRgnB, 0x4652), prev);
    }

    fill_alloc(heap, freeRgnT.end, (unsigned int)((char *)freeRgnB.start - (char *)freeRgnT.end));

    {
        MBlock *used;
        Region region;

        region.start = (char *)mem - sizeof(MBlock);
        region.end = freeRgnB.start;

        used = func_02068184(&region, 0x5544);

        set_alloc_dir(used, direction);
        set_alignment(used, (unsigned short)get_offset(freeRgnT.end, used));
        set_group_id(used, heap->groupID);

        func_02068154(&heap->usedList, used, heap->usedList.tail);
    }

    return mem;
}
