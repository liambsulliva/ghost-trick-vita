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

int func_ov004_020d7b90(struct Entry *e);
void func_ov004_020d8550(struct Entry *e);
void func_ov004_020d3a40(int *param_1, unsigned char *param_2);
void func_ov004_020d3a54(struct Entry *e, int src, int size, int dest);
void func_02072ea8(char *src, char *dest, int size);

int func_ov004_020d4b98(struct Dec *p, int dest)
{
    if (p->done2 == p->limit2) {
        return 0;
    }
    if (p->type == 0) {
        return 0;
    }
    if (p->type == 1) {
        p->entries[p->pos2].a = p->dst->cur;
        p->entries[p->pos2].b = dest;
        p->dst->cur += func_ov004_020d7b90(&p->entries[p->pos2]);
    } else if (p->type == 2) {
        p->entries[p->pos2].a = p->dst->cur;
        p->entries[p->pos2].b = dest;
        func_ov004_020d8550(&p->entries[p->pos2]);
        p->dst->cur += 0x28;
    } else if (p->type == 3) {
        if (p->fa4 == 1) {
            func_ov004_020d3a40((int *)&p->entries[p->pos2], (unsigned char *)p->dst->cur);
            p->dst->cur += 4;
        }
        func_ov004_020d3a54(&p->entries[p->pos2], p->dst->cur, 0x80, dest);
        p->dst->cur += 0x80;
    } else {
        func_02072ea8((char *)p->dst->cur, (char *)dest, 0x200);
        p->dst->cur += 0x200;
    }
    if (++p->pos2 == p->f1e) {
        p->pos2 = 0;
        p->fa4 = 0;
    }
    p->done2++;
    return 1;
}
