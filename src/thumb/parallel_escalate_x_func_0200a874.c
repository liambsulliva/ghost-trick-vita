struct Slot {
    unsigned char pad0[4];
    unsigned char value;
    unsigned char flag0 : 1;
    unsigned char pad5 : 7;
    signed char x0;
    signed char y0;
    unsigned short w;
    unsigned short h;
};

void func_0200a874(struct Slot *s, unsigned char *buf)
{
    unsigned short w;
    unsigned int t;
    unsigned int cb;
    unsigned int rb;
    unsigned int row;
    unsigned int col;
    unsigned short h;

    if (s->flag0 != 0) {
        return;
    }
    w = s->w;
    cb = (unsigned int)w >> 4;
    row = 0;
    h = s->h;
    rb = (unsigned int)h >> 4;
    while (row < rb) {
        if (row + s->y0 < 0x30) {
            col = 0;
            while (col < cb) {
                t = col + s->x0;
                if (t < 0x40) {
                    buf[(s->y0 + row) * 0x40 + t] = s->value;
                }
                col++;
            }
        }
        row++;
    }
}
