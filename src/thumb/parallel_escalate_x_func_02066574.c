typedef struct {
    int x;
    int y;
    int z;
} Vec;

typedef struct {
    int m[3][3];
} Mtx33;

typedef struct {
    unsigned char pad[0x28];
    Mtx33 mtx;
} Node;

typedef struct {
    unsigned char pad0[0x28];
    Vec pos;
    int sx;
    int sy;
    int pad3c;
    unsigned short angle;
    unsigned char pad42[0x3e];
    short a80;
    short a82;
    unsigned char pad84[4];
    unsigned short kind;
    unsigned char pad8a[0x22];
    void *ref;
    unsigned char padb0[4];
    Vec off;
    unsigned short base;
    unsigned char padc2[6];
    Node *node;
    unsigned char padcc[8];
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
} Obj;

extern Mtx33 data_020b048c;
extern void func_020668dc(Node *node, int flag);
extern void func_0202bcd4(Mtx33 *dst, Mtx33 *src);
extern void func_020668f4(Mtx33 *m, int x, int y, int z);
extern void func_0206e79c(Mtx33 *a, Mtx33 *b, Mtx33 *ab);
extern int func_02066904(int value);
extern int func_02066914(int value);
extern void func_020668fc(Mtx33 *m, int s, int c);
extern void func_0205958c(Vec *out, void *ref, int index);
extern Vec *func_020083a0(void *ref);
extern void func_020082d8(Vec *dst, Vec *src);
extern void func_02008378(Vec *out, Vec *a, Vec *b);
extern void func_020083b0(Vec *dst, Vec *src);
extern int func_02008350(void *ref);
extern void *func_02008444(void);
extern int func_0200f5c8(void *a);
extern void func_0202d700(Node *node, int id, int flag);
extern void func_020083c0(Vec *out, Vec *a, Vec *b);
extern int func_020452e8(Vec *v);
extern int func_0206ef28(int a, int b);
extern unsigned short func_0206f2a4(int value);

void func_02066574(Obj *p)
{
    Mtx33 scl1;
    Mtx33 rot1;
    Vec k;
    Vec h;
    Mtx33 m;
    Mtx33 scl2;
    Vec g;
    Vec f;
    Vec e;
    Vec d;
    Mtx33 rot2;
    Vec t2;
    Vec t1;
    int dist;
    int len;
    unsigned short ang;

    func_020668dc(p->node, p->b3);
    if (p->b3) {
        func_0202bcd4(&p->node->mtx, &data_020b048c);
        if (p->b4 && !p->b5) {
            func_020668f4(&scl1, -p->sx, p->sy, 0x1000);
        } else {
            func_020668f4(&scl1, p->sx, p->sy, 0x1000);
        }
        func_0206e79c(&p->node->mtx, &scl1, &p->node->mtx);
        if (p->angle != 0) {
            if (p->b4 && !p->b5) {
                func_020668fc(&rot1, func_02066904(p->angle), func_02066914(p->angle));
            } else {
                func_020668fc(&rot1, -func_02066904(p->angle), func_02066914(p->angle));
            }
            func_0206e79c(&p->node->mtx, &rot1, &p->node->mtx);
        }
        p->node->mtx.m[2][0] += p->pos.x + 0x80000;
        p->node->mtx.m[2][1] += 0x60000 - p->pos.y;
        return;
    }
    if (p->ref != 0) {
        func_0205958c(&k, p->ref, p->a80);
        func_020082d8(&h, func_020083a0(p->ref));
        func_02008378(&t1, &h, &k);
        func_02008378(&t2, &t1, &p->off);
        func_020083b0(&p->pos, &t2);
        if (func_02008350(p->ref) != 0) {
            p->b4 = 1 - p->b4;
        }
    }
    if (p->kind == 0x6d) {
        dist = p->pos.x - func_0200f5c8(func_02008444());
        if (dist < 0x40000) {
            p->b4 = 0;
        }
        if (dist > 0xc0000) {
            p->b4 = 1;
        }
    }
    func_0202d700(p->node, 5, p->b4);
    p->node->mtx.m[0][0] = 0x1000;
    p->node->mtx.m[0][1] = 0;
    p->node->mtx.m[0][2] = 0;
    p->node->mtx.m[1][0] = 0;
    p->node->mtx.m[1][1] = 0x1000;
    p->node->mtx.m[1][2] = 0;
    p->node->mtx.m[2][0] = p->pos.x;
    p->node->mtx.m[2][1] = p->pos.y;
    p->node->mtx.m[2][2] = 0x1000;
    m = data_020b048c;
    if (p->sx != 0x1000 || p->sy != 0x1000 || p->b4) {
        if (p->b4 && !p->b5) {
            func_020668f4(&scl2, -p->sx, p->sy, 0x1000);
        } else {
            func_020668f4(&scl2, p->sx, p->sy, 0x1000);
        }
        func_0206e79c(&m, &scl2, &m);
    }
    if (p->ref != 0 && p->a82 >= 0) {
        func_0205958c(&g, p->ref, p->a82);
        func_020082d8(&f, func_020083a0(p->ref));
        func_02008378(&e, &f, &g);
        func_020083c0(&d, &e, &p->pos);
        len = func_020452e8(&d);
        if (len > 0) {
            ang = func_0206f2a4(func_0206ef28(d.x, len));
            if (d.y > 0) {
                ang = -ang;
            }
            p->angle = p->base + ang;
        }
    }
    if (p->angle != 0) {
        if (p->b4 && !p->b5) {
            func_020668fc(&rot2, func_02066904(p->angle), func_02066914(p->angle));
        } else {
            func_020668fc(&rot2, -func_02066904(p->angle), func_02066914(p->angle));
        }
        func_0206e79c(&m, &rot2, &m);
    }
    func_0206e79c(&m, &p->node->mtx, &p->node->mtx);
}
