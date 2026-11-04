struct Board {
    char pad0[0xc];
    unsigned char *data;
    char pad10[0x14];
    unsigned char cell_w;
    unsigned char cell_h;
};

int func_0201ce60(struct Board *board, unsigned short x, unsigned short limit)
{
    unsigned int row;
    unsigned int cx;
    unsigned int col;
    unsigned int widest;
    unsigned char byte;
    unsigned char hi;
    unsigned char lo;

    if (x > limit) {
        return 1;
    }
    widest = 0;
    for (row = 0; row < board->cell_h; row++) {
        cx = x;
        for (col = 0; col < board->cell_w / 2; col++) {
            byte = board->data[(unsigned short)(col + row * board->cell_w / 2)];
            hi = byte & 0xf0;
            lo = byte & 0xf;
            if ((cx & 1) == 0) {
                if (limit <= cx && lo != 0 && widest < cx - limit + 1) {
                    widest = cx - limit + 1;
                }
                cx++;
            }
            if ((cx & 1) == 1) {
                if (limit <= cx && hi != 0 && widest < cx - limit + 1) {
                    widest = cx - limit + 1;
                }
                cx++;
            }
        }
    }
    return widest != 0;
}
