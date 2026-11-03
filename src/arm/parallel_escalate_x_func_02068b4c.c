typedef struct Block68b4c Block68b4c;

struct Block68b4c {
    unsigned int addr;
    unsigned int size;
    Block68b4c *prev;
    Block68b4c *next;
};

static inline void InsertBlock68b4c(Block68b4c **list, Block68b4c *b)
{
    if (*list != 0) {
        (*list)->prev = b;
    }
    b->next = *list;
    b->prev = 0;
    *list = b;
}

static inline void RemoveBlock68b4c(Block68b4c **list, Block68b4c *b)
{
    Block68b4c *prev = b->prev;
    Block68b4c *next = b->next;

    if (prev != 0) {
        prev->next = next;
    } else {
        *list = next;
    }
    if (next != 0) {
        next->prev = prev;
    }
}

static inline Block68b4c *TakeBlock68b4c(Block68b4c **unused)
{
    Block68b4c *b = *unused;

    if (b != 0) {
        *unused = b->next;
    }
    return b;
}

int func_02068b4c(Block68b4c **free_list, Block68b4c **unused, unsigned int *addr, unsigned int size, unsigned int align)
{
    unsigned int start;
    unsigned int need;
    unsigned int pad;
    Block68b4c *found;
    Block68b4c *b;
    Block68b4c *pad_block;

    found = 0;
    for (b = *free_list; b != 0; b = b->next) {
        if (align > 1) {
            start = (b->addr + (align - 1)) & ~(align - 1);
            pad = start - b->addr;
            need = size + pad;
        } else {
            start = b->addr;
            pad = 0;
            need = size;
        }
        if (b->size >= need) {
            found = b;
            break;
        }
    }
    if (found == 0) {
        goto fail;
    }
    if (pad != 0) {
        pad_block = TakeBlock68b4c(unused);
        if (pad_block == 0) {
            goto fail;
        }
        pad_block->addr = found->addr;
        pad_block->size = pad;
        pad_block->prev = 0;
        pad_block->next = 0;
        InsertBlock68b4c(free_list, pad_block);
    }
    found->size -= need;
    found->addr += need;
    if (found->size == 0) {
        RemoveBlock68b4c(free_list, found);
        InsertBlock68b4c(unused, found);
    }
    *addr = start;
    return 1;
fail:
    *addr = 0;
    return 0;
}
