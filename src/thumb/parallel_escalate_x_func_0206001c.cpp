struct Vec {
    int x;
    int y;
    int z;
    Vec() {}
    Vec(const Vec &src);
};

struct RopeFlags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

struct RopeLink {
    int a;
    int len;
};

struct Rope {
    unsigned char pad0[0x7c];
    RopeFlags flags;
    RopeFlags flags2;
    unsigned char state;
    unsigned char count;
    unsigned char pad80[4];
    unsigned short timer;
    unsigned char pad86[6];
    unsigned short scale_x;
    unsigned short scale_y;
    int vel_x;
    int vel_y;
    Vec anchor;
    unsigned char pada4[0xc];
    signed char wait;
    unsigned char padb1[3];
    int ticks;
    unsigned char fade;
    unsigned char padb9[3];
    Vec home;
    Vec *start;
    Vec *end;
    Vec *points;
    Vec *vel;
    unsigned char padd8[8];
    RopeLink *links;
    unsigned char pade4[0x18];
    int target;
    int owner;
};

extern "C" {
extern int data_0209db88[];
extern char data_0209dc1c[];
extern char data_0209dc20[];
extern char data_0209dc28[];
extern char data_0209dc34[];
extern char data_0209dc3c[];
extern char data_0209dc48[];
extern char data_0209dc5c[];
extern char data_020cfaf0[];

void func_020082d8(Vec *dst, const Vec *src);
Vec &func_020083b0(Vec &dst, const Vec &src);
Vec func_020083c0(const Vec &a, const Vec &b);
Vec func_02008378(const Vec &a, const Vec &b);
Vec func_020349b0(const Vec &v, int scale);
int func_020452e8(const Vec &v);
Vec func_0201f0a0(const Vec &v);
int func_0206f28c(int a, int b);
void func_0207303c(const void *src, void *dst, unsigned int size);
void func_0200f9cc(Vec *pos, Vec *vel);
void func_0200da04(char *a, char *b, char *c, char *d);
void *func_02008444(void);
int func_0201132c(void *game);
void func_02061448(Rope *p);
void func_020100f4(void *object, int value);
void func_02060d54(Rope *p, int state);
int func_02058b8c(int target);
void func_020276ec(int target, int value);
void *func_02009900(void);
int func_0202fb48(void *list, char *name, int index);
void func_0205e278(int obj, int target);
void func_0205e25c(int obj, int target);
void *func_0200847c(void);
void func_0202a0c0(void *sound, int id);
void func_02060f54(Rope *p, int value);
void *func_02008eb0(void);
unsigned int func_01ff80b8(void *object);

void func_0206001c(Rope *p, int a, int b)
{
    int fwd_len;
    int back_len;
    unsigned char i;
    unsigned char j;
    unsigned char n;
    int t;
    int obj;

    if (!p->flags.b1) {
        return;
    }
    if (p->state == 7) {
        p->ticks++;
        Vec first;
        func_020082d8(&first, p->start);
        Vec last;
        func_020082d8(&last, p->end);
        for (i = 1; i < p->count - 1; i++) {
            func_020083b0(p->points[i], func_020083c0(p->points[i], p->vel[i]));
        }
        func_020083b0(p->points[0], first);
        func_020083b0(p->points[p->count - 1], last);
        Vec span = func_020083c0(last, first);
        for (i = 1; i < p->count - 1; i++) {
            func_020083b0(p->points[i], func_02008378(p->points[0], func_020349b0(span, (i << 12) / (p->count - 1))));
            p->vel[i].x = data_0209db88[func_01ff80b8(func_02008eb0()) & 3];
            p->vel[i].y = data_0209db88[func_01ff80b8(func_02008eb0()) & 3];
            func_0200f9cc(&p->points[i], &p->vel[i]);
        }
        if (p->ticks >= 0x19) {
            p->ticks = 0;
            p->flags.b3 = 1;
        }
        return;
    }
    if (p->state == 6) {
        p->fade++;
        if (p->fade >= 0x30) {
            p->flags.b3 = 1;
        } else {
            t = (p->fade << 12) / 0x30;
            Vec mid = func_02008378(func_020349b0(p->home, 0x1000 - t), func_020349b0(p->points[0], t));
            Vec step = func_020083c0(mid, p->points[0]);
            step.x = step.x / p->count;
            step.y = step.y / p->count;
            for (i = 1; i < p->count - 1; i++) {
                p->points[i].x = p->points[0].x + step.x * i;
                p->points[i].y = p->points[0].y + step.y * i;
            }
            func_020083b0(p->points[p->count - 1], mid);
        }
        return;
    }
    if (p->state == 4 || p->state == 5 || p->state == 8) {
        for (i = 1; i < p->count - 1; i++) {
            func_020083b0(p->points[i], func_020083c0(p->points[i], p->vel[i]));
            p->vel[i].x = data_0209db88[func_01ff80b8(func_02008eb0()) & 3];
            p->vel[i].y = data_0209db88[func_01ff80b8(func_02008eb0()) & 3];
            func_0200f9cc(&p->points[i], &p->vel[i]);
        }
        if (p->timer == 0 && p->state == 4 && !p->flags2.b0) {
            func_0200da04(data_0209dc1c, data_0209dc20, data_0209dc28, data_0209dc34);
        }
        if (func_0201132c(func_02008444()) != 0 && p->state == 5) {
            if (p->wait != 0) {
                p->wait = p->wait - 1;
                if (p->wait == 0) {
                    func_02061448(p);
                }
            }
        } else {
            p->timer++;
            if (!p->flags2.b0) {
                if (p->timer == 4) {
                    p->flags.b7 = 1;
                }
            } else if (p->timer == 0x46) {
                p->flags2.b0 = 0;
                func_020100f4(func_02008444(), p->owner);
            }
        }
        return;
    }
    if (p->flags.b0) {
        if (p->state == 1) {
            func_02060d54(p, 2);
            if (p->target != 0 && func_02058b8c(p->target) != 0) {
                func_020276ec(p->target, 1);
                func_02061448(p);
                obj = func_0202fb48(func_02009900(), data_020cfaf0, -1);
                if (obj != 0) {
                    if (func_0201132c(func_02008444()) != 0) {
                        func_0205e278(obj, p->target);
                    } else {
                        func_0205e25c(obj, p->target);
                    }
                }
            }
            func_020100f4(func_02008444(), p->owner);
            p->target = p->owner;
            func_0202a0c0(func_0200847c(), 0x10);
            p->flags.b6 = 1;
            func_0200da04(data_0209dc1c, data_0209dc20, data_0209dc3c, data_0209dc34);
            if (func_0201132c(func_02008444()) != 0) {
                func_0200da04(data_0209dc1c, data_0209dc20, data_0209dc48, data_0209dc34);
            } else {
                func_0200da04(data_0209dc1c, data_0209dc20, data_0209dc5c, data_0209dc34);
            }
        } else if (p->state == 2) {
            func_020083b0(p->vel[p->count - 1], func_020083b0(p->points[p->count - 1], p->anchor));
            for (i = 0; i < p->count - 1; i++) {
                j = p->count - 2 - i;
                Vec back = func_020083c0(p->points[j], p->points[j + 1]);
                back_len = func_020452e8(back);
                Vec back_dir = func_0201f0a0(back);
                Vec back_ofs = func_020349b0(back_dir, func_0206f28c(back_len, p->links[i].len));
                func_020083b0(p->vel[j], func_02008378(p->vel[j + 1], back_ofs));
            }
            func_0207303c(p->vel, p->points, p->count * 12);
        }
    } else {
        if (p->flags2.b3 != 1 && ((a == 0 && b == 0 && p->state != 0) || p->state == 3)) {
            func_020083b0(p->anchor, func_020083b0(p->vel[0], p->points[0]));
            func_020083b0(p->vel[1], p->points[1]);
            for (i = 1; i < (n = p->count); i++) {
                Vec fwd = func_020083c0(p->points[i], p->points[i - 1]);
                fwd_len = func_020452e8(fwd);
                Vec fwd_dir = func_0201f0a0(fwd);
                Vec fwd_ofs = func_020349b0(fwd_dir, func_0206f28c(fwd_len, p->links[(unsigned char)(n - i)].len));
                func_020083b0(p->vel[i], func_02008378(p->vel[i - 1], fwd_ofs));
            }
            func_0207303c(p->vel, p->points, n * 12);
            func_02060d54(p, 3);
        }
    }
    if (!p->flags2.b3 && (p->flags.b0 || a == 0 || b == 0 || p->state == 3)) {
        Vec total = func_020083c0(p->points[p->count - 1], p->points[0]);
        if (func_020452e8(total) < 0x800) {
            func_02060d54(p, 0);
            func_02060f54(p, 0);
            p->scale_x = 0x400;
            p->scale_y = 0x400;
            p->vel_x = 0;
            p->vel_y = 0;
        }
    }
}
}
