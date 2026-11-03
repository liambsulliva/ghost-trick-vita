struct Vec {
    int x;
    int y;
    int z;
};

struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

class Talk {
public:
    virtual void t0();
    virtual void t1();
    virtual void t2();
    virtual void t3();
    virtual void t4();
    virtual void t5();
    virtual void t6();
    virtual void t7();
    virtual void t8();
    virtual void t9();
    virtual void t10();
    virtual void t11();
    virtual void t12();
    virtual void t13();
    virtual void t14();
    virtual void t15();
    virtual void t16();
    virtual void t17();
    virtual void t18();
    virtual void t19(int mode, int on);
};

struct Scene {
    char pad0[4];
    unsigned char state;
    unsigned char wait;
    unsigned char busy;
    unsigned char timer;
    char pad8[0x18];
    int vx;
    int vy;
    void *owner;
    Talk *talk;
    int mode;
};

extern "C" {
extern struct Pad data_020c9d50;
extern unsigned char data_020c9e88[];
extern char data_0209a6e0[];
extern char data_0209a6f8[];
extern char data_0209a700[];
extern char data_0209a710[];
extern void *func_02008444(void);
extern void *func_0200847c(void);
extern int func_02010b38(void *game);
extern int func_0201572c(void);
extern int func_020322d4(void *game);
extern int func_02013a14(void *game);
extern int func_0200f330(void *game);
extern void func_02010ef4(void *game, int id, int value);
extern void func_0201192c(void *game, int value);
extern void func_02010384(void *game, int value);
extern void func_02011598(void *game);
extern int func_02011264(void);
extern int func_0201123c(int object, int value);
extern void func_02022f34(int object, char *table, int index);
extern int func_02010fbc(void *game, int id);
extern void func_0202a0c0(void *sound, int id);
extern void func_0201135c(void *game);
extern int func_02032664(void *owner);
extern int func_02010194(void *game);
extern int func_020267a0(void *game);
extern void func_02058a80(struct Vec *dst, int node);
extern unsigned int func_020334f8(void *game);
extern int func_02036f0c(void *game);
extern int func_0200f5e8(void *game);
extern int func_02036f1c(void *game);
extern unsigned int func_020334ec(void *game);
extern int func_0200f5c8(void *game);
extern void func_020330e0(void *game, int value);
extern void func_020330c0(void *game, int value);
extern int func_0206f28c(int a, int b);
extern int func_02036f2c(int value);
extern void func_02034258(void *owner, int on);
extern void func_020113f8(void *game);
extern void func_02032644(void *game, int mode);
extern int func_02036f34(void *owner);
extern int func_0201132c(void *game);
extern void func_02034248(void *owner, int on);
extern int func_02036f3c(int node);
extern int func_020100e4(int node);
extern unsigned short *func_02058ab8(int node);
extern void func_02011690(void *game, int value);
extern int func_02027698(int node);
extern int func_020283c4(int node);
extern void func_02046ee0(int timer);
extern int func_02036f4c(void *owner);
extern void func_020112d4(void *game);
extern int func_02012518(void *game, char *a, char *b);
extern int func_0201230c(void *game);
extern void func_0202f060(struct Vec *a, int b);
extern void func_0200f958(void *game, int value);
extern void func_02035708(void *game);
extern int func_020330a4(void *game);
extern void func_0200f9e8(void *game, int value);
extern void func_0202682c(void *game);
extern int func_02031b20(struct Scene *p);
extern int func_02010f84(void *game, int id);
extern void func_02014260(void *game);
extern void func_0201424c(void *game);
extern void func_0202665c(void *game);
extern void *func_02010b7c(void);
extern unsigned int func_02015188(void *object);
extern int func_02015154(void *object);
extern int func_02010b60(void *object);
extern int func_02010b64(void *object);
extern int func_0201105c(void *game, int x, int y);
extern int func_020356f0(void *game);
extern int func_0200f1c0(void *game);
extern void func_020110b8(void *game);

void func_0203696c(struct Scene *p)
{
    struct Vec pos;
    struct Vec near;
    unsigned int trig;
    int node;
    unsigned short *info;

    trig = data_020c9d50.trig;
    if (p->talk == 0 && func_02010b38(func_02008444()) == 0) {
        if ((trig & 0x20000) != 0 && *(short *)(data_020c9e88 + 0x12) == 0 && func_0201572c() == 0 && func_020322d4(func_02008444()) != 0 && func_02013a14(func_02008444()) != 0 && func_0200f330(func_02008444()) == 0) {
            func_02010ef4(func_02008444(), 1, -1);
            func_0201192c(func_02008444(), 0x13);
            func_02010384(func_02008444(), 1);
            p->state = 4;
            func_02011598(func_02008444());
            func_02022f34(func_0201123c(func_02011264(), 0), data_0209a6e0, 0);
            return;
        }
        if ((trig & 0x400000) != 0 && func_02010fbc(func_02008444(), 5) != 0) {
            func_02010ef4(func_02008444(), 5, -1);
            func_0202a0c0(func_0200847c(), 0x11);
            return;
        }
    }
    if (p->busy != 0) {
        func_0201135c(func_02008444());
        p->busy = 0;
    }
    if (func_02032664(p->owner) != 0) {
        if (p->wait != 0) {
            p->wait--;
        }
        node = func_02010194(func_02008444());
        if (p->wait == 0 && node != 0 && func_020267a0(func_02008444()) == 0) {
            func_02058a80(&pos, node);
            if (func_020334f8(func_02008444()) > 0xc0) {
                p->vy = func_02036f0c(func_02008444());
                if (p->vy == 0) {
                    pos.y = pos.y - func_0200f5e8(func_02008444());
                    if (pos.y < 0x8000) {
                        p->vy = -0x4000;
                    } else if (pos.y > 0xb0000) {
                        p->vy = 0x4000;
                    }
                }
            } else {
                p->vy = 0;
            }
            if (func_02036f1c(func_02008444()) != 0) {
                if (func_020334ec(func_02008444()) > 0x100) {
                    pos.x = pos.x - func_0200f5c8(func_02008444());
                    if (pos.x < 0x10000) {
                        p->vx = -0x5800;
                    } else if (pos.x > 0xf0000) {
                        p->vx = 0x5800;
                    }
                } else {
                    p->vx = 0;
                }
                if (p->vx != 0) {
                    func_020330e0(func_02008444(), p->vx);
                }
                if (p->vy != 0) {
                    func_020330c0(func_02008444(), p->vy);
                }
                p->vx = func_0206f28c(p->vx, 0xf40);
                p->vy = func_0206f28c(p->vy, 0xf40);
                if (func_02036f2c(p->vx) < 0x400) {
                    p->vx = 0;
                }
                if (func_02036f2c(p->vy) < 0x40) {
                    p->vy = 0;
                }
            }
        }
        if (func_0200f330(func_02008444()) == 0 && func_02010b38(func_02008444()) == 0 && func_020267a0(func_02008444()) == 0) {
            if (p->talk != 0) {
                p->timer--;
                if (p->timer != 0) {
                    return;
                }
                p->talk->t19(p->mode, 0);
                p->talk = 0;
                func_02034258(p->owner, 0);
                func_020113f8(func_02008444());
                func_02032644(func_02008444(), 4);
                return;
            }
            if ((func_02036f34(p->owner) != 0 || (trig & 0x4000) != 0) && func_0201132c(func_02008444()) == 0 && func_02010fbc(func_02008444(), 4) != 0) {
                if (func_02036f34(p->owner) == 0) {
                    func_02010ef4(func_02008444(), 4, -1);
                }
                func_02034248(p->owner, 0);
                node = func_02010194(func_02008444());
                if (node == 0 || func_02036f3c(node) != 0 || func_020100e4(node) != 0) {
                    return;
                }
                info = func_02058ab8(node);
                if (info == 0 || info[2] == 0 || info[2] == 0x8000) {
                    func_0202a0c0(func_0200847c(), 4);
                    return;
                }
                func_02011690(func_02008444(), 1);
                p->talk = (Talk *)func_02027698(node);
                p->mode = func_020283c4(node);
                p->timer = 8;
                func_02034258(p->owner, 1);
                func_02046ee0(p->timer);
                p->talk->t19(p->mode, 1);
                return;
            }
            if ((func_02036f4c(p->owner) != 0 || (trig & 0x8000) != 0) && func_02010fbc(func_02008444(), 0) != 0) {
                func_02010384(func_02008444(), 1);
                func_020112d4(func_02008444());
                func_02011598(func_02008444());
                if (func_02012518(func_02008444(), data_0209a6f8, data_0209a700) == 0) {
                    node = func_02010194(func_02008444());
                    if (node != 0) {
                        func_02058a80(&near, node);
                        func_0202f060(&near, func_0201230c(func_02008444()));
                        if ((unsigned int)near.x > 0x100000 || (unsigned int)near.y > 0xc0000 || p->vx != 0 || p->vy != 0) {
                            func_0200f958(func_02008444(), 0x400000);
                            func_02035708(func_02008444());
                        }
                    }
                }
                p->state = 2;
                return;
            }
            if ((trig & 0x2000) != 0) {
                func_02011690(func_02008444(), 0);
                if (func_020330a4(func_02008444()) != 0) {
                    return;
                }
                func_0200f9e8(func_02008444(), 0x10000);
                return;
            }
            if ((trig & 0x10000) != 0 && func_02010fbc(func_02008444(), 3) != 0) {
                func_02010384(func_02008444(), 1);
                func_0202682c(func_02008444());
                func_02011598(func_02008444());
                func_02022f34(func_02031b20(p), data_0209a710, 0);
                func_02010ef4(func_02008444(), 3, -1);
                func_0202a0c0(func_0200847c(), 8);
                p->state = 3;
                return;
            }
            if ((trig & 0x1000000) != 0) {
                if (func_02010f84(func_02008444(), 6) == 0) {
                    return;
                }
                if (func_02010f84(func_02008444(), 7) == 0) {
                    return;
                }
                if (func_02010b38(func_02008444()) != 0) {
                    return;
                }
                func_02011598(func_02008444());
                p->busy = 1;
                func_0202a0c0(func_0200847c(), 0x1f);
                if (func_0201132c(func_02008444()) != 0) {
                    func_02010ef4(func_02008444(), 7, 0);
                    func_02014260(func_02008444());
                } else {
                    func_02010ef4(func_02008444(), 6, 1);
                    func_0201424c(func_02008444());
                }
                func_0202665c(func_02008444());
                func_0200f9e8(func_02008444(), 0x10000);
                return;
            }
            if ((trig & 0xf00) == 0 && (trig & 0x40000) == 0) {
                if (func_02015188(func_02010b7c()) != 0) {
                    return;
                }
                if (func_02015154(func_02010b7c()) == 0) {
                    return;
                }
                if (func_0201105c(func_02008444(), func_02010b60(func_02010b7c()), func_02010b64(func_02010b7c())) != 0) {
                    return;
                }
            }
            if (func_020267a0(func_02008444()) != 0 || func_020356f0(func_02008444()) != 0 || func_020330a4(func_02008444()) != 0 || func_0200f1c0(func_02008444()) != 0) {
                return;
            }
            p->state = 1;
            func_020110b8(func_02008444());
            return;
        }
    } else {
        p->vx = 0;
        p->vy = 0;
    }
}
}
