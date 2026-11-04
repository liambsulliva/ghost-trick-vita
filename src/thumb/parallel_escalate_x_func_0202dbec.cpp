class Res {
public:
    virtual void s0();
    virtual void s1();
    virtual void *s2(int size, void *name);
};

struct Item {
    unsigned char pad[0x6c];
};

struct Entry {
    unsigned char pad[0x10];
};

struct Source {
    unsigned char pad[0x14];
    unsigned int count;
    struct Item *items;
};

struct Info {
    unsigned char pad0[0xa];
    unsigned short xa;
    unsigned short xc;
    unsigned char xe;
    unsigned char padf[9];
    int x18;
    int x1c;
    int x20;
    int x24;
    int x28;
    int pad2c;
    int x30;
    int x34;
    int x38;
    int x3c;
};

class Layout {
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
    virtual void slot13(char *body);
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20(int a);

    unsigned char pad4[6];
    unsigned short x0a;
    unsigned char padc[0x138];
    unsigned int count;
    int *ids;
    unsigned int cnt_a;
    unsigned int cnt_c;
    unsigned int cnt_b;
    int cnt_d;
    int *x15c;
    int *x160;
    int x164;
    int x168;
    int x16c;
    int x170;
    int x174;
    int x178;
    unsigned short x17c;
    unsigned short x17e;
    unsigned char pad180[2];
    unsigned short x182;
    unsigned char pad184[8];
    unsigned char bit0 : 1;
    unsigned char pad18d[0x57];
    unsigned char marks[3];
    unsigned char pad1e7;
    struct Entry *entries;
    int total;
    struct Item *items;
    struct Source *parent;
};

extern "C" {
extern char data_02085298[];
extern struct Item *func_0207dc50(int count, int size, int align, void *ctor, void *dtor);
extern void func_0202dfa0(void);
extern void func_0202dbdc(void);
extern void func_0202dfb8(struct Item *item, char *src);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *system);
extern void func_0202dfbc(struct Entry *entry, int kind);
extern void func_0202b2ac(struct Entry *entry, char *src);
extern void func_0202dfc0(struct Entry *entry, unsigned short value);
extern int func_0202dfc4(struct Entry *entry);
extern int func_0202c820(struct Item *item);
void func_0202dbec(Layout *p, int *h, struct Source *src);
}

void func_0202dbec(Layout *p, int *h, struct Source *src)
{
    struct Info *info;
    int b;
    int offc;
    int offb;
    int *hdr4;
    char *base;
    char *data;
    char *secb;
    char *secc;
    int cntc;
    int *offsa;
    char *bodya;
    char *bodyb;
    char *bodyc;
    int a;
    int c;
    int d;
    unsigned int i;
    unsigned int k;
    int offa;
    char *seca;
    int *offsb;
    int *offsc;
    int ca;
    int cb;
    int c1;
    int *secd;
    int cntd;
    unsigned int m;

    p->parent = src;
    hdr4 = h + 1;
    base = (char *)(hdr4 + h[0]);
    info = (struct Info *)(base + 8);
    data = (char *)info + info->x28;
    a = info->x18;
    p->x164 = a;
    b = info->x1c;
    p->x168 = b;
    c = info->x20;
    p->x16c = c;
    d = info->x24;
    p->x170 = d;
    p->x182 = info->xc;
    p->bit0 = info->xe;
    p->x174 = (b - a) >> 1;
    p->x178 = (d - c) >> 1;
    p->x17c = info->xa;
    p->x0a = p->x182;
    p->x17e = 1;
    if (p->x17c != 0) {
        p->x17e = 60 / p->x17c;
    }
    if (p->parent == 0) {
        p->count = *(int *)data;
        p->items = func_0207dc50(p->count, 0x6c, 8, (void *)func_0202dfa0, (void *)func_0202dbdc);
        for (i = 0; i < p->count; i++) {
            func_0202dfb8(&p->items[i], base + hdr4[i + 1]);
        }
    } else {
        p->count = p->parent->count;
    }
    ca = *(int *)(data + info->x30);
    seca = data + info->x30;
    p->cnt_a = ca;
    secb = data + info->x34;
    cb = *(int *)secb;
    p->cnt_b = cb;
    secc = data + info->x38;
    cntc = *(int *)secc;
    c1 = cntc - 1;
    p->cnt_c = c1;
    p->total = ca + cb + c1;
    p->entries = (struct Entry *)func_02007b70(func_02007bd8())->s2(p->total * 16, data_02085298);
    p->ids = (int *)(data + 4) + p->count;
    offsa = (int *)seca + 1;
    bodya = (char *)(offsa + p->cnt_a);
    for (i = 0; i < p->cnt_a; i++) {
        offa = offsa[i];
        func_0202dfbc(&p->entries[i], 2);
        func_0202b2ac(&p->entries[i], bodya + offa);
        func_0202dfc0(&p->entries[i], 0xffff);
        for (k = 0; k < p->count; k++) {
            if (p->ids[k] == func_0202dfc4(&p->entries[i])) {
                if ((p->items != 0 && func_0202c820(&p->items[k]) != 0) || (p->parent != 0 && func_0202c820(&p->parent->items[k]) != 0)) {
                    func_0202dfc0(&p->entries[i], k);
                }
                break;
            }
        }
    }
    offsb = (int *)secb + 1;
    bodyb = (char *)(offsb + p->cnt_b);
    for (k = 0; k < p->cnt_b; k++) {
        offb = offsb[k];
        func_0202dfbc(&p->entries[k + p->cnt_a], 7);
        func_0202b2ac(&p->entries[k + p->cnt_a], bodyb + offb);
    }
    offsc = (int *)secc + 1;
    bodyc = (char *)(offsc + cntc);
    p->slot13(bodyc);
    for (k = 0; k < p->cnt_c; k++) {
        offc = offsc[k + 1];
        func_0202dfbc(&p->entries[k + p->cnt_a + p->cnt_b], 0x27);
        func_0202b2ac(&p->entries[k + p->cnt_a + p->cnt_b], bodyc + offc);
    }
    secd = (int *)(data + info->x3c);
    cntd = *secd;
    p->cnt_d = cntd;
    secd++;
    p->x15c = secd;
    secd += cntd;
    p->x160 = secd;
    p->slot20(cntd);
    for (m = 0; m < 3; m++) {
        p->marks[m] = 0;
    }
}
