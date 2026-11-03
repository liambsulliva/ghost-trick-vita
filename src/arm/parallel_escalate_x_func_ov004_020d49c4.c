typedef int code();

struct Obj {
    code **vt;
};

struct Entry {
    int a;
    int b;
    char pad[0x14f0];
};

struct Out {
    int cur;
    int pad[2];
    int x[6];
    int y[6];
};

struct Dec {
    struct Obj *obj;
    int pad04[2];
    unsigned int total;
    int f10;
    unsigned int f14;
    int pad18;
    unsigned short type;
    unsigned short f1e;
    int pad20[5];
    struct Out *dst;
    int pad38;
    int desc[4];
    int pad4c[3];
    struct Entry *entries;
    int *tabA;
    int *tabB;
    int *tabC;
    int bufA;
    int bufB;
    int a[2];
    int b[2];
    int c[2];
    int d[2];
    int idx;
    int f94;
    unsigned int count;
    unsigned int cnt;
    unsigned int max;
    int fa4;
    unsigned int n;
    int k[6];
    int pos;
    unsigned int limit2;
    unsigned int done2;
    unsigned int pos2;
};

struct Params {
    int a;
    int b;
    int c;
    int d;
    int e;
    unsigned int f;
    int g;
    int h;
    int i;
};

int func_ov004_020d6b58(int param_1);
int func_ov004_020d4f00(void);
void func_ov004_020e4098(struct Params *s);
void func_ov004_020d5138(int *desc);

int func_ov004_020d49c4(struct Dec *p, int a1, int a2, int mode)
{
    struct Params s;

    if (p->cnt >= p->max) {
        return 0;
    }
    if (mode != 0) {
        if (p->bufA == 0) {
            p->bufA = func_ov004_020d6b58(p->f14 << 8);
            if (p->bufA == 0) {
                return 0;
            }
        }
        if (p->bufB == 0) {
            p->bufB = func_ov004_020d6b58((p->f14 >> 1) << 8);
            if (p->bufB == 0) {
                return 0;
            }
        }
        s.a = p->tabA[p->pos];
        s.b = p->tabB[p->pos];
        s.c = p->bufA;
        s.d = p->bufB;
        s.e = p->f10;
        s.f = p->f14;
        s.g = p->tabC[p->pos];
        s.h = func_ov004_020d4f00();
        if (mode == 2) {
            s.i = 0;
        } else {
            s.i = 1;
        }
        func_ov004_020e4098(&s);
        p->desc[0] = p->bufA;
        p->desc[1] = p->bufB;
    } else {
        p->desc[0] = p->tabA[p->pos];
        p->desc[1] = p->tabB[p->pos];
    }
    p->desc[2] = a1;
    p->desc[3] = a2 << 1;
    func_ov004_020d5138(p->desc);
    p->cnt++;
    if (++p->pos == p->n) {
        p->pos = 0;
    }
    return 1;
}
