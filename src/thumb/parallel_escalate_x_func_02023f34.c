typedef struct Node23f34 {
    int f0;
    int value;
    unsigned int bits : 24;
    unsigned int top : 8;
    int used;
    unsigned int handle;
} Node23f34;

typedef struct Table23f34 {
    char pad[0x20];
    Node23f34 *slots[0xa0];
} Table23f34;

unsigned int func_02023f34(Table23f34 *table, Node23f34 *node, int value, unsigned int bits, unsigned int handle)
{
    Node23f34 **slot = &table->slots[1];
    unsigned int i;
    for (i = 1; i < 0xa0; i++, slot++) {
        if (*slot == 0) {
            node->used = 1;
            node->value = value;
            node->bits = bits;
            node->handle = (handle & 0xfffffc00) | (i & 0x3ff);
            *slot = node;
            return node->handle;
        }
    }
    return 0;
}
