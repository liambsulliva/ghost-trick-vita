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

extern int data_020cff7c;
extern struct List data_020cff80;
extern void func_02067eb4(struct List *list, unsigned short offset);
extern struct List *func_0206802c(struct HeapHead *heap);
extern void func_02067ef8(struct List *list, void *object);

void func_02068054(struct HeapHead *heap, unsigned int signature, void *heapStart, void *heapEnd, unsigned short optFlag)
{
    heap->signature = signature;
    heap->heapStart = heapStart;
    heap->heapEnd = heapEnd;
    heap->attribute = 0;
    heap->attribute = (heap->attribute & ~0xff) | (optFlag & 0xff);
    func_02067eb4(&heap->childList, 4);
    if (data_020cff7c == 0) {
        func_02067eb4(&data_020cff80, 4);
        data_020cff7c = 1;
    }
    func_02067ef8(func_0206802c(heap), heap);
}
