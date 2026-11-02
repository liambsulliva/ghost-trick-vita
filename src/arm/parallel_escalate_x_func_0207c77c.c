typedef struct Index0207c7e0 {
    unsigned int start;
    unsigned int flags;
    unsigned int table;
} Index0207c7e0;

Index0207c7e0 *func_0207c77c(Index0207c7e0 *first, int count, unsigned int pc)
{
    int lo;
    int hi;
    int mid;
    Index0207c7e0 *e;

    hi = count - 1;
    lo = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        e = first + mid;
        if (pc < e->start) {
            hi = mid - 1;
        } else if (pc > e->start + (e->flags & ~1)) {
            lo = mid + 1;
        } else {
            return e;
        }
    }
    return 0;
}
