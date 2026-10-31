typedef struct {
    unsigned int first;
    unsigned int last;
    unsigned short count;
    unsigned short offset;
} List020226e8;

typedef struct {
    unsigned int next;
    unsigned int previous;
    int addr;
    unsigned int size : 24;
    unsigned int tag : 8;
} Block020226e8;

typedef struct {
    int pad0;
    int align;
    int pad8;
    int padc;
    List020226e8 used;
    List020226e8 free;
    List020226e8 spare;
} Alloc020226e8;

extern Block020226e8 *func_020227e0(Alloc020226e8 *a, int addr);
extern Block020226e8 *func_02067fac(List020226e8 *list, Block020226e8 *node);
extern void func_02067f4c(List020226e8 *list, Block020226e8 *node);
extern void func_02067ef8(List020226e8 *list, Block020226e8 *node);

void func_020226e8(Alloc020226e8 *a, int addr)
{
    Block020226e8 *b;
    Block020226e8 *f;

    b = func_020227e0(a, addr);
    if (b == 0) {
        return;
    }
    if (b->tag == 1) {
        return;
    }
    func_02067f4c(&a->used, b);
    for (f = func_02067fac(&a->free, 0); f != 0; f = func_02067fac(&a->free, f)) {
        if (b->addr == f->addr + f->size) {
            f->size = f->size + b->size;
            func_02067f4c(&a->free, f);
            func_02067ef8(&a->spare, b);
            b = f;
            break;
        }
    }
    for (f = func_02067fac(&a->free, 0); f != 0; f = func_02067fac(&a->free, f)) {
        if (f->addr == b->addr + b->size) {
            f->addr = f->addr - b->size;
            f->size = f->size + b->size;
            func_02067f4c(&a->free, f);
            func_02067ef8(&a->spare, b);
            b = f;
            break;
        }
    }
    func_02067ef8(&a->free, b);
}
