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

unsigned short func_ov004_020d6b98(int param_1);

void func_ov004_020d44d8(struct Dec *p)
{
    unsigned int i;
    int j;

    if (p->obj != 0) {
        (*p->obj->vt[6])(p->obj);
        if (p->obj != 0) {
            (*p->obj->vt[1])(p->obj);
        }
    }
    func_ov004_020d6b98((int)p->dst);
    if (p->tabA != 0) {
        for (i = 0; i < p->n; i++) {
            func_ov004_020d6b98(p->tabA[i]);
        }
        func_ov004_020d6b98((int)p->tabA);
    }
    if (p->tabB != 0) {
        for (i = 0; i < p->n; i++) {
            func_ov004_020d6b98(p->tabB[i]);
        }
        func_ov004_020d6b98((int)p->tabB);
    }
    if (p->tabC != 0) {
        func_ov004_020d6b98((int)p->tabC);
    }
    if (p->bufA != 0) {
        func_ov004_020d6b98(p->bufA);
    }
    if (p->bufB != 0) {
        func_ov004_020d6b98(p->bufB);
    }
    for (j = 0; j < 2; j++) {
        if (p->a[j] != 0) {
            func_ov004_020d6b98(p->a[j]);
        }
        p->a[j] = 0;
    }
    if (p->f94 != 0) {
        func_ov004_020d6b98(p->f94);
    }
    if (p->entries != 0) {
        func_ov004_020d6b98((int)p->entries);
    }
    p->obj = 0;
    p->tabA = 0;
    p->tabB = 0;
    p->bufA = 0;
    p->bufB = 0;
    p->tabC = 0;
    p->entries = 0;
    p->f94 = 0;
}
