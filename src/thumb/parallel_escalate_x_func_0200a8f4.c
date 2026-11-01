struct Map {
    unsigned char pad0[0x7a04];
    int grid[48][64];
    unsigned char pad1[0x2a];
    unsigned short count;
    unsigned char pad2[0xe];
    unsigned char width;
    unsigned char height;
};

struct Filler {
    unsigned char pad0[0x10];
    unsigned char done;
    unsigned char pad11[0x17];
    struct Map *map;
    int x;
    int y;
    int dx;
    int row;
    int wait;
};

extern int func_02009ddc(void);
extern int func_0200a5dc(struct Map *m);
extern void func_02009c1c(void);
extern unsigned int func_0200aa14(void);
extern int func_0200aa20(void);
extern void func_0200a4a0(struct Map *m, int x, int y);

void func_0200a8f4(struct Filler *p)
{
    int n;
    int r;

    if (p->wait != 0) {
        p->wait--;
        return;
    }
    if (p->map == 0) {
        return;
    }
    if (func_02009ddc() == 0) {
        return;
    }
    if (func_0200a5dc(p->map) != 0) {
        if (p->done == 0) {
            p->done = 1;
        }
        return;
    }
    if (p->map->count > 0x10) {
        return;
    }
    func_02009c1c();
    if (func_0200aa14() > 0x8000) {
        return;
    }
    n = 16;
    while (n != 0) {
        r = func_0200aa20();
        if (r < 0xc0 && r > 0xa8) {
            return;
        }
        if (p->map->grid[p->y][p->x] == 0) {
            func_0200a4a0(p->map, p->x, p->y);
            n--;
            if (func_0200a5dc(p->map) != 0) {
                p->done = 1;
                return;
            }
        }
        p->x += p->dx;
        if (p->x >= p->map->width) {
            p->dx = -p->dx;
            p->x = p->map->width - 1;
            p->y -= 4;
        }
        if (p->x < 0) {
            p->dx = -p->dx;
            p->x = 0;
            p->y -= 4;
        }
        if (p->y < 0) {
            p->row = (p->row + 1) & 3;
            p->y = p->map->height - 1 - p->row;
            p->x = 0;
            p->dx = 1;
            return;
        }
    }
}
