typedef struct Cell Cell;
struct Cell {
    Cell *prev;
    Cell *next;
    long size;
};

typedef struct {
    long size;
    Cell *free;
    Cell *allocated;
} HeapDesc;

typedef struct {
    volatile int currentHeap;
    int numHeaps;
    void *arenaStart;
    void *arenaEnd;
    HeapDesc *heapArray;
} OSHeapInfo;

extern OSHeapInfo *data_020d1188[];
extern Cell *func_02074cec(Cell *list, Cell *cell);
extern Cell *func_02074d08(Cell *list, Cell *cell);
extern int func_02075c98(void);
extern int func_02075cac(int enabled);

void *func_02074dd8(int id, int heap, unsigned int size)
{
    OSHeapInfo *heapInfo;
    HeapDesc *hd;
    Cell *cell;
    Cell *newCell;
    long leftoverSize;
    int enabled = func_02075c98();

    heapInfo = data_020d1188[id];
    if (!heapInfo) {
        func_02075cac(enabled);
        return 0;
    }

    if (heap < 0) {
        heap = heapInfo->currentHeap;
    }

    hd = &heapInfo->heapArray[heap];

    size += 32;
    size = (size + 31) & ~31;

    for (cell = hd->free; cell != 0; cell = cell->next) {
        if ((long)size <= cell->size) {
            break;
        }
    }

    if (cell == 0) {
        func_02075cac(enabled);
        return 0;
    }

    leftoverSize = cell->size - (long)size;
    if (leftoverSize < sizeof(Cell) + 52) {
        hd->free = func_02074d08(hd->free, cell);
    } else {
        cell->size = (long)size;

        newCell = (Cell *)((char *)cell + size);
        newCell->size = leftoverSize;

        newCell->prev = cell->prev;
        newCell->next = cell->next;
        if (newCell->next != 0) {
            newCell->next->prev = newCell;
        }
        if (newCell->prev != 0) {
            newCell->prev->next = newCell;
        } else {
            hd->free = newCell;
        }
    }

    hd->allocated = func_02074cec(hd->allocated, cell);

    func_02075cac(enabled);
    return (void *)((char *)cell + 32);
}
