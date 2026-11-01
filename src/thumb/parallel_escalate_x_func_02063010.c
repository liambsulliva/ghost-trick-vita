typedef struct {
    int x;
    int y;
    int z;
} Vec;

typedef struct {
    short x0;
    short y0;
    short x1;
    short y1;
} Seg;

struct Arc {
    unsigned char pad0[0x28];
    Vec from;
    unsigned char pad34[0x48];
    Vec to;
    int radius;
    int scale;
    unsigned char pad90[2];
    unsigned char shown;
    unsigned char fixed;
    unsigned char pad94[4];
    Seg *segs;
};

extern int data_0209dd44[];
extern void func_02056cb8(struct Arc *p);
extern int func_0202c358(struct Arc *p);
extern void func_020082d8(Vec *dst, Vec *src);
extern void *func_02008444(void);
extern int func_0200f5c8(void *a);
extern int func_0200f5e8(void *a);
extern int func_0206f28c(int a, int b);

void func_02063010(struct Arc *p)
{
    Vec a;
    Vec b;
    int r;
    int q;
    unsigned int i;
    int k;
    int x;
    int y;

    func_02056cb8(p);
    if (func_0202c358(p) == 0 || p->shown == 0) {
        return;
    }
    func_020082d8(&a, &p->from);
    func_020082d8(&b, &p->to);
    if (p->fixed != 0) {
        a.x -= 0x80000;
        a.y = 0x60000 - a.y;
        b.x -= 0x80000;
        b.y = 0x60000 - b.y;
    } else {
        a.x = a.x - func_0200f5c8(func_02008444()) - 0x80000;
        a.y = -a.y + func_0200f5e8(func_02008444()) + 0x60000;
        b.x = b.x - func_0200f5c8(func_02008444()) - 0x80000;
        b.y = -b.y + func_0200f5e8(func_02008444()) + 0x60000;
    }
    a.x >>= 7;
    a.y = a.y / 3 >> 5;
    b.x >>= 7;
    b.y = b.y / 3 >> 5;
    r = func_0206f28c(p->radius, p->scale);
    q = p->radius * 3 >> 2;
    for (i = 0; i < 0x11; i++) {
        k = i * 2 + 0x20;
        if (k >= 0x40) {
            k -= 0x40;
        }
        x = b.x + func_0206f28c(q, data_0209dd44[k + 1]);
        y = b.y + func_0206f28c(r, data_0209dd44[k]);
        p->segs[i].x0 = a.x;
        p->segs[i].x1 = x;
        p->segs[i].y0 = a.y;
        p->segs[i].y1 = y;
    }
}
