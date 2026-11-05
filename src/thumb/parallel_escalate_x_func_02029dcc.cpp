class Res {
public:
    virtual void s0();
    virtual void s1();
    virtual void *s2(int size, void *name);
};

struct Head {
    unsigned char b0;
    unsigned char pad1;
    unsigned short h2;
    int w4;
    int w8;
    int wc;
};

struct Rec {
    unsigned char b0;
    unsigned char pad1;
    unsigned short h2;
    int w4;
    int w8;
};

struct Range {
    unsigned short h0;
    unsigned short pad2;
    int w4;
    int w8;
};

struct Pair {
    unsigned short h0;
    unsigned short pad2;
    int w4;
};

struct Slot {
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char pad3;
    int d;
    int e;
};

struct Scene {
    void *vt;
    void *o4;
    void *o8;
    void *oc;
    void *o10;
    int w14;
    int w18;
    void *w1c;
    void *w20;
    unsigned char b24;
    unsigned char b25;
    unsigned short h26;
    unsigned short h28;
    unsigned short h2a;
    unsigned short h2c;
    unsigned short h2e;
    int w30;
    int w34;
    struct Head head;
    struct Rec recs[32];
    struct Range range;
    struct Pair pairs[2];
    struct Slot slots[8];
    int w244;
};

extern "C" {
extern char data_020a501c;
extern char data_02085220;
extern char data_02085224;
extern char data_02085234;
extern char data_02085248;
extern char data_02085258;
extern void func_02007aa8(struct Scene *p);
extern void *func_02031254(int size);
extern void *func_0200c504(void *object);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *a);
extern void func_0201e144(void *object, void *data, int size, int mode);
extern void *func_02029d38(struct Scene *p, int size);
extern void func_0200c530(void *object, void *name);
extern void func_0201f288(void *manager, void *object);
extern int func_0201e4d8(void *object);
extern int func_02006ecc(void);
extern int func_02029d4c(struct Scene *p, int a, int b);
extern void func_02006e84(int a, int b);
extern int func_02006edc(void *name);
extern void func_02006fac(void *name, int w);
extern int func_02006ee4(void);
extern void func_02029da4(struct Scene *p, int a);
extern void func_02006fc0(void);
extern void func_02007100(void);
extern void func_0202a00c(struct Scene *p);

struct Scene *func_02029dcc(struct Scene *p)
{
    void *object;
    int size;
    int i;
    struct Head *head;
    unsigned char *b;
    struct Rec *recs;
    struct Range *range;
    struct Pair *pairs;
    struct Slot *slots;
    int w;

    func_02007aa8(p);
    p->vt = &data_020a501c;
    p->b24 = p->b25 = 0;
    p->w14 = 0;
    p->w18 = 0;
    p->h26 = 0;
    p->h28 = 0;
    p->h2a = 0;
    p->h2c = 0;
    p->h2e = 0;
    p->w34 = 0;
    p->w30 = 0;
    head = &p->head; p->head.b0 = 0; head->h2 = 0xffff; head->w4 = 0; head->w8 = 0; head->wc = 0;
    for (i = 0; i < 32; i++) { recs = &p->recs[i]; recs->b0 = 0; recs->h2 = 0xffff; recs->w4 = 0; recs->w8 = 0; }
    range = &p->range; range->h0 = 0xffff; range->w4 = 0; range->w8 = 0;
    for (i = 0; i < 2; i++) { pairs = &p->pairs[i]; pairs->h0 = 0xffff; pairs->w4 = 0; }
    for (i = 0; i < 8; i++) { slots = &p->slots[i]; slots->a = 0; slots->b = 0; slots->c = 0; slots->d = 0; slots->e = 0; }
    object = func_02031254(0x3c);
    if (object != 0) {
        object = func_0200c504(object);
    }
    p->o4 = object;
    object = func_02031254(0x3c);
    if (object != 0) {
        object = func_0200c504(object);
    }
    p->o8 = object;
    object = func_02031254(0x3c);
    if (object != 0) {
        object = func_0200c504(object);
    }
    p->oc = object;
    object = func_02031254(0x3c);
    if (object != 0) {
        object = func_0200c504(object);
    }
    p->o10 = object;
    func_0201e144(p->o4, func_02007b70(func_02007bd8())->s2(-0x100000, &data_02085220), 0x100000, 0x20);
    func_0201e144(p->o8, func_02029d38(p, 0xefec), 0xefec, 0x20);
    func_0200c530(p->o8, &data_02085224);
    func_0201f288(func_02007bd8(), p->o8);
    p->w1c = func_02029d38(p, 0x70fec);
    func_0201e144(p->oc, p->w1c, 0x70fec, 0x20);
    func_0200c530(p->oc, &data_02085234);
    func_0201f288(func_02007bd8(), p->oc);
    size = func_0201e4d8(p->o4) - 0x14;
    p->w20 = func_02029d38(p, size);
    func_0201e144(p->o10, p->w20, size, 0x20);
    func_0200c530(p->o10, &data_02085248);
    func_0201f288(func_02007bd8(), p->o10);
    p->w14 = w = func_02029d4c(p, func_02006ecc(), 0);
    func_02006e84(1, w);
    p->w18 = w = func_02029d4c(p, func_02006edc(&data_02085258), 0);
    func_02006fac(&data_02085258, w);
    func_02029da4(p, func_02006ee4());
    func_02006fc0();
    func_02007100();
    p->w244 = 0;
    func_0202a00c(p);
    return p;
}
}
