struct Board {
    char pad0[0x10];
    unsigned short width;
    unsigned short height;
};

void func_0201cdd0(struct Board *board, unsigned char *base, unsigned int x, unsigned int y, unsigned char color)
{
    unsigned char hi;
    unsigned char lo;
    unsigned char *p;
    unsigned char cur;
    unsigned int off;
    unsigned char high;

    if (x >= board->width || y >= board->height) {
        return;
    }
    if ((x & 1) == 0) {
        off = (x + y * board->width) / 2;
        cur = base[off];
        p = base + off;
        hi = cur & 0xf0;
        lo = cur & 0xf;
        if (color != 0) {
            lo = color;
        }
    } else {
        off = (x - 1 + y * board->width) / 2;
        cur = base[off];
        p = base + off;
        lo = cur & 0xf;
        hi = cur & 0xf0;
        high = color << 4;
        if (high != 0) {
            hi = high;
        }
    }
    *p = hi | lo;
}
