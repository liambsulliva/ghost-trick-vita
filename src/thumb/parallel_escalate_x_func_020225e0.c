typedef struct {
    unsigned int first;
    unsigned int last;
    unsigned short count;
    unsigned short offset;
} List020225e0;

typedef struct {
    unsigned int next;
    unsigned int previous;
    int addr;
    unsigned int size : 24;
    unsigned int tag : 8;
} Block020225e0;

typedef struct {
    int pad0;
    int align;
    int pad8;
    int padc;
    List020225e0 used;
    List020225e0 free;
    List020225e0 spare;
} Alloc020225e0;

extern Block020225e0 *func_02067fac(List020225e0 *list, Block020225e0 *node);
extern void func_02067f4c(List020225e0 *list, Block020225e0 *node);
extern void func_02067ef8(List020225e0 *list, Block020225e0 *node);

int func_020225e0(Alloc020225e0 *a, int size, unsigned char tag)
{
    unsigned char neg;
    int result;
    Block020225e0 *node;
    Block020225e0 *n;
    unsigned int mask;

    neg = 0;
    result = -1;
    if (size < 0) {
        neg = 1;
        size = -size;
    }
    mask = a->align - 1;
    size = ~mask & (size + mask);
    node = func_02067fac(&a->free, 0);
    while (node != 0) {
        if (size == node->size) {
            func_02067f4c(&a->free, node);
            func_02067ef8(&a->used, node);
            break;
        }
        if (size < node->size) {
            n = func_02067fac(&a->spare, 0);
            if (n == 0) {
                return -1;
            }
            if (neg) {
                n->addr = node->addr + node->size - size;
                n->size = size;
                node->size = node->size - size;
            } else {
                n->addr = node->addr;
                n->size = size;
                node->addr = node->addr + size;
                node->size = node->size - size;
            }
            node = n;
            func_02067f4c(&a->spare, n);
            func_02067ef8(&a->used, node);
            break;
        }
        node = func_02067fac(&a->free, node);
    }
    if (node != 0) {
        node->tag = tag;
        result = node->addr;
    }
    return result;
}
