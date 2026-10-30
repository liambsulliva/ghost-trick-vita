
unsigned int *func_0206812c(unsigned int *head, unsigned int *node)
{
    unsigned int *prev;
    unsigned int *next;

    prev = (unsigned int *)node[2];
    next = (unsigned int *)node[3];
    if (prev != 0) {
        prev[3] = (unsigned int)next;
    } else {
        head[0] = (unsigned int)next;
    }
    if (next != 0) {
        next[2] = (unsigned int)prev;
    } else {
        head[1] = (unsigned int)prev;
    }
    return prev;
}
