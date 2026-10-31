struct Cell {
    int a;
    int b;
};
struct Board {
    int pad0;
    struct Cell *cells;
    unsigned char pad8[0x24 - 8];
    unsigned short step;
    unsigned char pad26[0x30 - 0x26];
    short x;
    short y;
};
extern void func_02018b8c(struct Cell *cell, int x, int y, int a, int b, int c, int d, unsigned int e, int f, int g, int h, int k);

void func_02018b18(struct Board *board)
{
    int count;
    unsigned int row;
    int col;
    int i;

    count = 0;
    row = 0;
    col = 0;
    do {
        for (i = 0; (unsigned int)i < 8; i++) {
            func_02018b8c(board->cells + (i + row * 8), board->x + i * 0x20, board->y + col * 0x10,
                          1, 0, 0, 0, 0x80004000, 0, count * board->step, 0, 0);
            count++;
        }
        row++;
        col++;
    } while (row < 3);
}
