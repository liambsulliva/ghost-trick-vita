struct Vec {
    int x;
    int y;
    int z;
};

struct Clock {
    unsigned char x0;
    unsigned char rate;
    signed char wait;
    signed char flag;
    int frame;
};

class Obj {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33(int value);
};

struct Graph {
    int x0;
    Obj obj;
    unsigned char pad8[0xc4];
    int x[64];
    int y[64];
    int w[64];
    int vx[64];
    int vy[64];
    unsigned char idx[64];
    int tx[64];
    unsigned int count;
    unsigned char pad710;
    unsigned char on;
    unsigned char fade;
    signed char mode;
    short fade_in;
    short fade_out;
    short fade_time;
    short level;
    short hold;
    short delay;
    int cycle;
    int speed;
    int x728;
    int ma;
    int mb;
    int mc;
    int x738;
    int na;
    int nb;
    int nc;
    int x748;
    int wa;
    int wb;
    int wc;
    int wd;
    int x75c;
    int sa;
    int sb;
    int sc;
    int tb;
    int tc;
    int ua;
    int ub;
    int uc;
    int va;
    int vb;
    int vc;
};

extern "C" {
extern struct Clock data_020cff60;
extern int data_020af568;
extern int data_020af564;
extern int data_020af560;
extern int data_020af55c;
extern int data_020af558;
extern int data_020af554;
extern void *func_02008444(void);
extern int func_0205d0c0(void *system);
extern int func_0205f040(void *system);
extern int func_0205f060(void *system);
extern int func_0205f054(Graph *p);
extern void func_0205eaec(Graph *p, int on);
extern void func_020083c0(struct Vec *dst, struct Vec *a, struct Vec *b);
extern void func_0200f70c(struct Vec *v);
extern void func_0201f0a0(struct Vec *dst, struct Vec *src);
extern int func_0205f13c(Graph *p, int *a, int *b, int *c, int *d, short *e);
extern int func_0206f28c(int a, int b);
void func_0205eb78(Graph *p, int num, int extra, struct Vec *pts, struct Vec *base);
}

void func_0205eb78(Graph *p, int num, int extra, struct Vec *pts, struct Vec *base)
{
    struct Vec d;
    struct Vec e;
    struct Vec f;
    Graph *row;
    unsigned int i;
    unsigned int n;
    int k;
    int s1;
    int ex;
    int ey;
    int s2;
    int s3;
    int s4;
    int s5;
    int s6;
    int dv;

    if (func_0205d0c0(func_02008444()) != 0) {
        p->obj.slot33(0x390);
    } else {
        p->obj.slot33(0x910);
    }
    if (p->fade_in > 0) {
        p->fade = p->fade_time * 0x1f / p->fade_in;
        p->fade_time = p->fade_time + 1;
        if (p->fade_time > p->fade_in) {
            p->fade_in = 0;
        }
    }
    if (p->fade_out > 0) {
        p->fade = 0x1f - 0x1f * p->fade_time / p->fade_out;
        p->fade_time = p->fade_time + 1;
        if (p->fade_time > p->fade_out) {
            p->fade_out = 0;
        }
    }
    if (p->mode == 2) {
        if (func_0205f040(func_02008444()) != 0) {
            if (func_0205f054(p) != 0) {
                func_0205eaec(p, 0);
            }
        } else {
            if (func_0205f054(p) == 0) {
                func_0205eaec(p, 1);
            }
        }
    }
    if (p->mode == 1) {
        if (func_0205f060(func_02008444()) != 0) {
            if (func_0205f054(p) != 0) {
                func_0205eaec(p, 0);
            }
        } else {
            if (func_0205f054(p) == 0) {
                func_0205eaec(p, 1);
            }
        }
    }
    n = 0;
    if (p->hold > 0) {
        p->hold--;
    }
    if (p->level > 0) {
        p->delay--;
        if (p->delay <= 0) {
            p->level--;
            p->delay = 6;
        }
    } else if (p->cycle > 0) {
        p->delay--;
        if (p->delay <= 0) {
            p->cycle++;
            p->delay = 6;
            if (p->cycle > 0x40) {
                p->cycle = 0;
            }
        }
    }
    for (i = 0; i < p->count; i++) {
        row = (Graph *)((char *)p + i * 4);
        row->x[0] += row->vx[0];
        row->y[0] += row->vy[0];
        dv = (row->tx[0] - row->x[0]) >> 1;
        dv *= data_020cff60.rate + 1;
        row->vx[0] += dv;
        row->w[0] -= p->speed * (data_020cff60.rate + 1);
        if (row->w[0] <= 0) {
            row->w[0] = 0;
            if (p->on != 0 && p->hold <= 0) {
                n++;
                if (n >= p->cycle && p->cycle > 0 && p->level > 0) {
                    p->hold = p->level;
                }
                k = p->idx[i];
                func_020083c0(&d, &pts[k + 1], &pts[k]);
                func_0200f70c(&e);
                e.x = d.y;
                e.y = -d.x;
                e.z = d.z;
                func_0201f0a0(&f, &e);
                s1 = func_0205f13c(p, &p->ua, &p->ub, &p->uc, &data_020af568, &p->level);
                ex = func_0206f28c(e.x, s1);
                ey = func_0206f28c(e.y, s1);
                s2 = func_0205f13c(p, &p->va, &p->vb, &p->vc, &data_020af564, &p->level);
                s3 = func_0205f13c(p, &p->sa, &p->sb, &p->sc, &data_020af560, &p->level);
                s4 = func_0205f13c(p, &p->sa, &p->tb, &p->tc, &data_020af55c, &p->level);
                row->x[0] = base[k].x + func_0206f28c(d.x, s2);
                row->y[0] = base[k].y + func_0206f28c(d.y, s2);
                row->x[0] += ex + s3;
                row->y[0] += ey + s4;
                row->w[0] = func_0205f13c(p, &p->wa, &p->wb, &p->wc, &p->wd, &p->level);
                s5 = func_0205f13c(p, &p->ma, &p->mb, &p->mc, &data_020af558, &p->level);
                s6 = func_0205f13c(p, &p->na, &p->nb, &p->nc, &data_020af554, &p->level);
                row->vx[0] = s5 * (data_020cff60.rate + 1);
                row->vy[0] = -s6 * (data_020cff60.rate + 1);
                row->tx[0] = row->x[0];
            }
        }
    }
}
