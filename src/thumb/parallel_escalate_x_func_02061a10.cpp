class Marker {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int a);
    unsigned char pad4[0x28];
    unsigned short slot;
    unsigned char pad2e[6];
    int serial;
    unsigned char pad38[0x20];
    unsigned char active : 1;
    unsigned char follow : 1;
    unsigned char hidden : 1;
    unsigned char tracked : 1;
    unsigned char second : 1;
    unsigned char first : 1;
    unsigned char pad59[3];
    int x;
    int y;
    int node;
    int offset;
    int wait;
};

typedef struct {
    int x;
    int y;
    int z;
} Vec;

extern "C" {
extern void *func_0200847c(void);
extern int func_02061b78(void *a, unsigned char b);
extern int func_02061b8c(void *a, unsigned char b);
extern int func_02061bb4(void *a, unsigned char b);
extern int func_02061b9c(void *a, int b);
extern void *func_02008444(void);
extern int func_0200f330(void *a);
extern int func_02011320(void *a);
extern void *func_0202f25c(void);
extern int func_02061bd4(void *a);
extern void func_0202ac74(void *a, unsigned char b, int c, int d);
extern int func_020083a0(int a);
extern void func_0205958c(Vec *v, int a, int b);
extern void func_02008378(Vec *dst, int m, Vec *src);
extern void func_0202abf8(void *a, unsigned char b, int c, int d);
void func_02061a10(Marker *p);
}

void func_02061a10(Marker *p)
{
    void *view;
    int m;
    Vec w;
    Vec v;

    if (!p->second) {
        if (!p->first) {
            p->first = 1;
        } else {
            p->second = 1;
        }
    }
    if (func_02061b78(func_0200847c(), p->slot) == 0 || p->serial != func_02061b8c(func_0200847c(), p->slot)) {
        p->s8(0);
        return;
    }
    if (!p->tracked) {
        return;
    }
    view = func_0200847c();
    if (func_02061b9c(view, func_02061bb4(func_0200847c(), p->slot)) == 0) {
        if (func_0200f330(func_02008444()) != 0 && func_02011320(func_02008444()) != 0x12 && func_02061bd4(func_0202f25c()) == 0) {
            if (!p->hidden) {
                p->hidden = 1;
                func_0202ac74(func_0200847c(), p->slot, 0, 0x1e);
            }
        } else if (p->hidden) {
            p->hidden = 0;
            func_0202ac74(func_0200847c(), p->slot, 0x100, 0x1e);
        }
    }
    if (p->active) {
        p->wait--;
        if (p->wait <= 0) {
            p->wait = 4;
            if (p->follow) {
                m = func_020083a0(p->node);
                func_0205958c(&v, p->node, p->offset);
                func_02008378(&w, m, &v);
                p->x = w.x;
                p->y = w.y;
            }
            func_0202abf8(func_0200847c(), p->slot, p->x, p->y);
        }
    }
}
