class Sprite {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13(int x, int y, int z);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33(int value);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int a, int b);
};

class Res {
public:
    virtual void r0();
    virtual void r1();
    virtual void r2();
    virtual void *r3(int size);
};

struct Entry {
    short f0;
    unsigned short sound;
    short f4;
    short x;
    short y;
    char pad[6];
};

struct Ids {
    unsigned short id[25];
};

struct Rect {
    int v[4];
};

struct Info {
    unsigned short lo : 2;
    unsigned short index : 14;
};

struct Glyph {
    unsigned short a;
    unsigned short b;
    int pad;
};

struct Scene {
    char pad0[0x20];
    int busy;
    char pad24;
    signed char kind;
    char pad26[0x32];
    Sprite *first;
    Sprite *second;
    Sprite *third;
};

extern "C" {
extern struct Entry data_ov006_020dacdc[];
extern struct Ids data_ov006_020d88fc;
extern struct Rect data_ov006_020d88d8;
extern struct Info data_02088910[];
extern struct Glyph data_0208c064[];
extern char data_020cd8cc[];
extern char data_020cf724[];
extern char data_020c9e74[];
extern void *func_02008444(void);
extern void func_0200ecb0(void *game, int sound, int value);
extern void func_0200f1a8(void *game, int value);
extern void func_02011a14(void *game, int value);
extern void func_02011a28(void *game, int value);
extern void func_020267ac(void *game, int value);
extern void func_020267b8(void *game, int value);
extern void *func_02009c1c(void);
extern void func_0202f3a4(void *system, int a, int b);
extern Sprite *func_02033da0(void *heap);
extern Sprite *func_0205693c(void *heap);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, Sprite *obj, int a, int b);
extern unsigned int func_ov006_020d3fd4(void *a);
extern void func_0205765c(Sprite *obj, int a, int b, int c, int d);
extern void func_0200ec24(void *game, int value);
extern void *func_0200883c(void);
extern void *func_02008824(void *system);
extern void *func_0201e888(void *font, int a, int id, int b, int c);
extern int func_0200cb5c(void *object);
extern void func_0202f1bc(void *list, void *data, int a, int b, int c);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *system);
extern void *func_02031254(unsigned int size);
extern void *func_020299ec(void *buf);
extern void func_02029af4(void *buf, void *mem, int id, int a, int b, int c);
extern void func_02029a04(void *buf);
extern void func_02031270(void *buf);
extern void func_ov006_020d4600(struct Rect *r);
extern void func_0206f588(void *dst, struct Rect *r, int x, int y, int a, int b);

void func_ov006_020d42d8(struct Scene *p, int mode, int id)
{
    struct Ids ids;
    struct Rect rect;
    unsigned int base;
    int index;
    struct Glyph *g;
    void *a;
    int b;
    void *c;
    int d;
    void *mem;
    void *buf;

    if (mode == 0) {
        func_0200ecb0(func_02008444(), data_ov006_020dacdc[id].sound, 0x40000);
        func_0200f1a8(func_02008444(), 0x1000);
        func_02011a14(func_02008444(), data_ov006_020dacdc[id].x << 12);
        func_02011a28(func_02008444(), data_ov006_020dacdc[id].y << 12);
        func_020267ac(func_02008444(), data_ov006_020dacdc[id].x << 12);
        func_020267b8(func_02008444(), data_ov006_020dacdc[id].y << 12);
        if (p->busy == 0) {
            if (id == 0x2c) {
                func_0202f3a4(func_02009c1c(), 0x10, 1);
            } else {
                func_0202f3a4(func_02009c1c(), 0x10, 0);
            }
            func_0202f3a4(func_02009c1c(), 4, 0);
        }
        if (p->first == 0) {
            p->first = func_02033da0(data_020cd8cc);
            p->first->s33(0x300);
            p->first->s13(-0x40000, 0, 0);
            func_0202f7e8(func_02009900(), p->first, 1, 0);
        }
        base = func_ov006_020d3fd4(func_02008444());
        if (p->second == 0) {
            p->second = func_0205693c(data_020cf724);
            p->second->s33(0x2e0);
            p->second->s42(0x50, 0, 1, 0, 1, -1, base + 0x6c000);
            p->second->s13(-0x10000, 0, 0);
            func_0202f7e8(func_02009900(), p->second, 1, 0);
        }
        if (p->third == 0) {
            p->third = func_0205693c(data_020cf724);
            p->third->s42(0x51, 0, 1, 0, 1, -1, 0);
            p->third->s33(0x2f0);
            p->third->s46(0x80000, 0xc0000);
            func_0205765c(p->third, 0, 0, 0x80000, 0xc0000);
            p->third->s13(0x40000, 0, 0);
            func_0202f7e8(func_02009900(), p->third, 1, 0);
        }
    } else {
        func_0202f3a4(func_02009c1c(), 0x10, 0);
        func_0202f3a4(func_02009c1c(), 4, 0);
        func_0200ec24(func_02008444(), 1);
        ids = data_ov006_020d88fc;
        index = data_02088910[ids.id[p->kind]].index;
        g = &data_0208c064[index];
        a = func_0201e888(func_02008824(func_0200883c()), 0, g->a, 1, 0);
        b = func_0200cb5c(func_02008824(func_0200883c()));
        c = func_0201e888(func_02008824(func_0200883c()), 3, g->b, 1, 0);
        d = func_0200cb5c(func_02008824(func_0200883c()));
        func_0202f1bc(func_02009c1c(), a, 0x6000, b, 0x10);
        func_0202f1bc(func_02009c1c(), c, 0, d, 0xb);
        mem = func_02007b70(func_02007bd8())->r3(0x8000);
        buf = func_02031254(0x410);
        if (buf != 0) {
            buf = func_020299ec(buf);
        }
        func_02029af4(buf, mem, ids.id[p->kind], 0x400, 0x400, 0);
        if (buf != 0) {
            if (buf != 0) {
                func_02029a04(buf);
                func_02031270(buf);
            }
        }
        func_0202f1bc(func_02009c1c(), mem, 0, 0x8000, 0xf);
        rect = data_ov006_020d88d8;
        func_ov006_020d4600(&rect);
        func_0206f588(data_020c9e74, &rect, 0x80, 0x60, 0, 0);
    }
}
}
