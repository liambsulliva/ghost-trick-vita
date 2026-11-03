typedef struct Range1e474 {
    int f0;
    unsigned int count;
    unsigned int stride;
    unsigned int start;
} Range1e474;

int func_0201e474(Range1e474 *range, unsigned int value)
{
    unsigned int span = range->stride * range->count;
    if (range->start <= value && value < range->start + span) {
        return 1;
    }
    return 0;
}
