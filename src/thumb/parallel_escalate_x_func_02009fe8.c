extern void func_0200a644(unsigned char *p, unsigned char v);

void func_02009fe8(unsigned char *p, unsigned int a, unsigned int b, int c, int d)
{
    unsigned char *cell;
    unsigned int row;
    unsigned int col;
    unsigned char *base;
    unsigned int cols;
    unsigned int rows;
    unsigned int sbd;
    unsigned int sac;

    cols = (unsigned int)*(unsigned short *)(p + 0xaa28) >> 4;
    rows = (unsigned int)*(unsigned short *)(p + 0xaa2a) >> 4;
    sbd = b + d;
    sac = a + c;
    base = p + 0x6202;
    for (row = 0; row < rows; row++) {
        cell = base + row * 0x40;
        if ((b > row) || (sbd <= row)) {
            for (col = 0; col < cols; col++) {
                func_0200a644(p + 0x100, cell[col]);
                cell[col] = 0;
            }
        } else {
            for (col = 0; col < cols; col++) {
                if ((a > col) || (sac <= col)) {
                    func_0200a644(p + 0x100, cell[col]);
                    cell[col] = 0;
                }
            }
        }
    }
}
