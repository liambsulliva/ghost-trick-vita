typedef union {
    struct {
        unsigned short attr0;
        unsigned short attr1;
        unsigned short attr2;
        unsigned short attr3;
    } h;
    struct {
        unsigned int y : 8;
        unsigned int mode : 6;
        unsigned int shape : 2;
        unsigned int x : 9;
        unsigned int param : 5;
        unsigned int size : 2;
        unsigned int tile : 10;
        unsigned int rest : 22;
    } b;
} Oam;

struct Ref {
    unsigned short kind : 2;
    unsigned short index : 14;
};

struct Res {
    int data;
    unsigned short id;
    unsigned short flags;
};

extern struct Ref data_02088910[];
extern struct Res data_0208c064[];
extern unsigned char data_020851f0[4][4][2];
extern void *func_0200883c(void);
extern void *func_02008824(void *a);
extern unsigned short *func_0201e888(void *a, int b, int c, int d, int e);
extern unsigned int func_02029d04(Oam *o);
extern unsigned int func_02029d14(Oam *o);
extern void *func_02007bd8(void);
extern int **func_02007b70(void *a);

void func_02029af4(int unused, unsigned short *dst, int id, unsigned int w, unsigned int h, int base)
{
    struct Res *cell;
    unsigned short *res;
    unsigned short *src;
    int n;
    unsigned int wt;
    unsigned int ht;
    unsigned int cols;
    unsigned int rows;
    unsigned int ty;
    unsigned int y0;
    unsigned int x0;
    unsigned int yy;
    unsigned int xx;
    unsigned int tx;
    int tile;
    Oam oam;
    int **mgr;

    cell = &data_0208c064[data_02088910[id].index];
    res = func_0201e888(func_02008824(func_0200883c()), 3, cell->id, 0, 0);
    src = res + res[0];
    n = *src++;
    wt = w >> 3;
    ht = h >> 3;
    while (n != 0) {
        oam.h.attr0 = src[0];
        oam.h.attr1 = src[1];
        oam.h.attr2 = src[2];
        cols = data_020851f0[oam.b.shape][oam.b.size][0];
        rows = data_020851f0[oam.b.shape][oam.b.size][1];
        tile = base;
        tile += oam.b.tile * 2;
        x0 = func_02029d04(&oam) + 0x80;
        y0 = func_02029d14(&oam) + 0x60;
        for (yy = 0; yy < rows; yy += 8) {
            for (xx = 0; xx < cols; xx += 8) {
                tx = (x0 + xx) >> 3;
                ty = (y0 + yy) >> 3;
                if (tx < wt && ty < ht) {
                    dst[wt * ty + tx] = tile;
                }
                tile++;
            }
        }
        src += 3;
        n--;
    }
    if (res != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            mgr = func_02007b70(func_02007bd8());
            ((void (*)(int **, unsigned short *))(*mgr)[4])(mgr, res);
        }
    }
}
