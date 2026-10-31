typedef struct {
    char pad0[0x7a04];
    int grid[48][64];
    char pad1[0x3a];
    unsigned char width;
    unsigned char height;
} Board;

extern char data_020b42bc;
extern unsigned char *func_02009900();
extern unsigned char *func_0202fb48();
extern int func_0200a5cc();
extern int func_0200a5dc(Board *p);
extern void func_0200a4a0(Board *p, int x, int y);

void func_0200a524(Board *p)
{
    unsigned char *obj;
    int x;
    int y;
    int dir;
    unsigned int phase;

    obj = func_02009900();
    obj = func_0202fb48(obj, &data_020b42bc, 0xe);
    if (obj != 0) {
        if (func_0200a5cc(obj) != 0) {
            return;
        }
    }
    if (func_0200a5dc(p) != 0) {
        return;
    }
    x = 0;
    dir = 1;
    y = p->height - 1;
    phase = 0;
    while (func_0200a5dc(p) == 0) {
        if (p->grid[y][x] == 0) {
            func_0200a4a0(p, x, y);
        }
        x += dir;
        if (x >= p->width) {
            dir = -dir;
            x = p->width - 1;
            y -= 4;
        }
        if (x < 0) {
            dir = -dir;
            x = 0;
            y -= 4;
        }
        if (y < 0) {
            x = 0;
            phase = (phase + 1) & 3;
            dir = 1;
            y = p->height - 1 - phase;
        }
    }
}
