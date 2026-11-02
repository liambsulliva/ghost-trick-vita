typedef struct Block020689a8 {
    unsigned int start;
    unsigned int size;
    struct Block020689a8 *prev;
    struct Block020689a8 *next;
} Block020689a8;

static inline void RemoveBlock(Block020689a8 **list, Block020689a8 *block)
{
    Block020689a8 *prev;
    Block020689a8 *next;

    prev = block->prev;
    next = block->next;
    if (prev != 0) {
        prev->next = next;
    } else {
        *list = next;
    }
    if (next != 0) {
        next->prev = prev;
    }
}

static inline void AddFront(Block020689a8 **list, Block020689a8 *block)
{
    if (*list != 0) {
        (*list)->prev = block;
    }
    block->next = *list;
    block->prev = 0;
    *list = block;
}

int func_020689a8(Block020689a8 **free, Block020689a8 **used, unsigned int *range)
{
    Block020689a8 *block;
    Block020689a8 *next;
    int merged;

    merged = 0;
    block = *free;
    while (block != 0) {
        next = block->next;
        if (block->start == range[1]) {
            merged |= 1;
            range[1] = block->start + block->size;
            RemoveBlock(free, block);
            AddFront(used, block);
        }
        if (range[0] == block->start + block->size) {
            range[0] = block->start;
            RemoveBlock(free, block);
            merged |= 1;
            AddFront(used, block);
        }
        block = next;
    }
    return merged;
}
