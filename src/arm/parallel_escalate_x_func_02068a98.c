typedef struct Node68a98 {
    int f0;
    int f4;
    struct Node68a98 *prev;
    struct Node68a98 *next;
} Node68a98;

Node68a98 *func_02068a98(Node68a98 *nodes, unsigned int count)
{
    unsigned int i;
    for (i = 0; i < count - 1; i++) {
        nodes[i].next = &nodes[i + 1];
        nodes[i + 1].prev = &nodes[i];
    }
    nodes[0].prev = 0;
    (nodes + count - 1)->next = 0;
    return nodes;
}
