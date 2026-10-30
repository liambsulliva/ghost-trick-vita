int func_0201236c(char *object, int value)
{
    int low;
    unsigned int i;
    int limit;
    char *p;
    int high;

    limit = value >> 12;
    for (i = 0; i < 4; i++) {
        p = object + i * 2;
        low = *(unsigned short *)(p + 6);
        if (low > limit) {
            high = *(unsigned short *)(p + 14);
            if (high < limit) {
                low -= 0xc0;
                if (low < 0) {
                    low = 0;
                }
                return low << 12;
            }
        }
    }
    limit -= 0x60;
    if (limit < 0) {
        limit = 0;
    }
    return limit << 12;
}
