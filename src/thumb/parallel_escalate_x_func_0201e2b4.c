typedef struct Range1e2b4 {
    char pad[0x28];
    unsigned int start;
    unsigned int size;
} Range1e2b4;

int func_0201e2b4(Range1e2b4 *range, unsigned int value)
{
    if (range->start <= value && value < range->start + range->size) {
        return 1;
    }
    return 0;
}
