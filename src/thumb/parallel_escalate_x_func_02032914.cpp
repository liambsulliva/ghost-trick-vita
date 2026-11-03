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

class Actor {
public:
    virtual void a0();
    virtual void a1();
    virtual void a2();
    virtual void a3();
    virtual void a4();
    virtual void a5();
    virtual void a6();
    virtual void a7();
    virtual void a8();
    virtual void a9();
    virtual void a10();
    virtual void a11();
    virtual void a12(struct Vec *pos);
};

struct Field {
    char pad0[4];
    unsigned char opened;
    unsigned char count;
    char pad6[2];
    int wait;
    char padc[0x14];
    void *owner;
    Actor *actor;
    char pad28[9];
    unsigned char busy;
    unsigned char hold;
    char pad33[2];
    unsigned char lock;
    unsigned char mode;
    unsigned char seen;
};

extern "C" {
extern struct Pad data_020c9d50;
extern unsigned char data_020c9e88[];
extern char data_020cfe64[];
extern char data_020cfb90[];
extern char data_020cd7d0[];
extern char data_02099dd4[];
extern char data_02099dfc[];
extern char data_02099e00[];
extern char data_02099e90[];
extern char data_02099e9c[];
extern char data_02099ea8[];
extern char data_02099eb8[];
extern char data_02099ec4[];
extern char data_02099ed8[];
extern char data_02099eec[];
extern char data_02099ef8[];
extern char data_02099f00[];
extern char data_02099f08[];
extern char data_02099f10[];
extern char data_02099f28[];
extern char data_02099f3c[];
extern char data_02099f48[];
extern void *func_02008444(void);
extern void *func_0200847c(void);
extern void *func_02009900(void);
extern int func_020267a0(void *game);
extern void func_02061138(Actor *actor, int value);
extern int func_02011320(void *game);
extern Actor *func_02065b08(char *name);
extern int func_02010194(void *game);
extern void func_02058a80(struct Vec *dst, int node);
extern void func_0202f7e8(void *list, Actor *actor, int a, int b);
extern void func_0202a0c0(void *sound, int id);
extern int func_02012518(void *game, char *a, char *b);
extern void func_02012110(void *game, int value);
extern int func_0202fa8c(void *list, char *name);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern int func_0203247c(void *game);
extern void func_0201192c(void *game, int value);
extern void func_0200f084(void *game, int a, int b, int c, int d);
extern void func_020120b0(void *game, int a, int b, int c);
extern int func_02031b20(struct Field *p);
extern void func_02022f34(int object, char *table, int index);
extern void func_02022f2c(int object, char *table, int index);
extern Actor *func_0202fb48(void *list, char *name, int index);
extern Actor *func_0205e8a8(char *name);
extern int func_02058b64(int node);
extern int func_020330a4(void *game);
extern void *func_02010b7c(void);
extern int func_02015154(void *object);
extern int func_02061074(Actor *actor);
extern void func_02033358(struct Field *p);
extern void func_02033428(struct Vec *out, struct Field *p, struct Vec *pos, int flag);
extern int func_0200f5c8(void *game);
extern int func_0200f5e8(void *game);
extern int func_020330b4(void *game);
extern void func_020330c0(void *game, int value);
extern void func_020330e0(void *game, int value);
extern int func_020328e4(void *game);
extern void func_020113f8(void *game);
extern int func_02033100(void *game);
extern void *func_02008860(void);
extern void func_0200c82c(void *sys, int a);
extern void func_0201135c(void *game);
extern int func_0201132c(void *game);
extern void func_02010e7c(void *game);
extern void func_02010e5c(void *game);
extern void func_02010ebc(void *game);
extern int func_02010b38(void *game);
extern void func_0202a1b0(void *sound, int index, int flag);
extern void func_02010ef4(void *game, int id, int value);
extern void func_02011598(void *game);
extern void func_02061230(Actor *actor);
extern int func_02033408(struct Field *p);
extern int func_020333e4(struct Field *p);
extern void func_02033110(void *owner);
extern void func_0200f958(void *game, int value);
extern int func_02033124(Actor *actor);
extern int func_02010fbc(void *game, int id);
extern void func_02014260(void *game);
extern void func_0201424c(void *game);
extern int func_02011264(void);
extern int func_0201123c(int object, int value);
extern void func_02033138(void *owner);

void func_02032914(struct Field *p)
{
    struct Vec pos;
    struct Vec goal;
    struct Vec near;
    struct Vec spot;
    Actor *obj;
    void *game;
    int busy;
    int node;
    int dx;
    int dy;
    int vx;
    int vy;

    if (p->wait == 1) {
        if (func_020267a0(func_02008444()) == 0) {
            p->hold = 0;
            p->wait = 0;
            func_02061138(p->actor, 1);
        }
        return;
    }
    switch (func_02011320(func_02008444())) {
    case 0x13:
        return;
    case 6:
        if (p->count == 0) {
            return;
        }
        p->count = 0xf;
        break;
    case 0x15:
        func_02061138(p->actor, 0);
        p->count = 0;
        break;
    case 0x14:
        if (func_020267a0(func_02008444()) != 0) {
            return;
        }
        if (p->opened == 0) {
            p->opened = 1;
            obj = func_02065b08(data_020cfe64);
            if (obj == 0) {
                return;
            }
            func_02058a80(&spot, func_02010194(func_02008444()));
            obj->a12(&spot);
            func_0202f7e8(func_02009900(), obj, 1, 0);
            func_0202a0c0(func_0200847c(), 0x1c);
            if (func_02012518(func_02008444(), data_02099e90, data_02099e9c) == 0) {
                func_02012110(func_02008444(), 0);
            }
            return;
        }
        if (func_0202fa8c(func_02009900(), data_020cfe64) == 0) {
            if (func_02012518(func_02008444(), data_02099e90, data_02099ea8) != 0) {
                func_0200da04(data_02099dfc, data_02099e90, data_02099ea8, data_02099e00);
                game = func_02008444();
                func_0201192c(game, func_0203247c(func_02008444()));
                break;
            }
            func_0200f084(func_02008444(), 1, 1, 1, 1);
            if (func_02012518(func_02008444(), data_02099e90, data_02099eb8) != 0) {
                func_0200da04(data_02099dfc, data_02099e90, data_02099eb8, data_02099e00);
            } else {
                func_020120b0(func_02008444(), 9, 0x100, 0);
            }
            func_02022f34(func_02031b20(p), data_02099ec4, 0);
        }
        return;
    default:
        if (p->actor == 0) {
            p->actor = func_0202fb48(func_02009900(), data_020cfb90, -1);
            if (p->actor == 0) {
                p->actor = func_0205e8a8(data_020cfb90);
                func_0202f7e8(func_02009900(), p->actor, 1, 0);
            }
        }
        busy = 0;
        node = func_02010194(func_02008444());
        if (node != 0) {
            busy = func_02058b64(node);
        }
        if (p->hold == 0 && func_020330a4(func_02008444()) == 0 && busy == 0) {
            if ((func_02015154(func_02010b7c()) != 0 || (data_020c9d50.cur & 0xf00) != 0) && func_02061074(p->actor) != 0 && p->mode == 0) {
                func_02033358(p);
            } else {
                node = func_02010194(func_02008444());
                if (node != 0) {
                    func_02058a80(&pos, node);
                    func_02033428(&goal, p, &pos, 1);
                    dx = goal.x - func_0200f5c8(func_02008444());
                    dy = goal.y - func_0200f5e8(func_02008444());
                    vx = 0;
                    vy = 0;
                    if (dy < 0x5c000) {
                        vy = -0x4000;
                    } else if (dy > 0x64000) {
                        vy = 0x4000;
                    }
                    if (dx < 0x7c000) {
                        vx = -0x4000;
                    } else if (dx > 0x84000) {
                        vx = 0x4000;
                    }
                    node = func_020330b4(func_02008444());
                    if (p->mode != 0 && node != 0) {
                        func_02058a80(&near, node);
                        dx = near.x - func_0200f5c8(func_02008444()) - 0x80000;
                        dy = near.y - func_0200f5e8(func_02008444()) - 0x60000;
                        if (dx < -0x70000 && vx > 0) {
                            vx = 0;
                        }
                        if (dx > 0x70000 && vx < 0) {
                            vx = 0;
                        }
                        if (dy < -0x50000 && vy > 0) {
                            vy = 0;
                        }
                        if (dy > 0x50000 && vy < 0) {
                            vy = 0;
                        }
                        if (dx < -0x78000) {
                            vx = -0x4000;
                        }
                        if (dx > 0x78000) {
                            vx = 0x4000;
                        }
                        if (dy < -0x58000) {
                            vy = -0x4000;
                        }
                        if (dy > 0x58000) {
                            vy = 0x4000;
                        }
                    }
                    func_020330c0(func_02008444(), vy);
                    func_020330e0(func_02008444(), vx);
                }
            }
        }
        if (p->count != 0 && func_020328e4(func_02008444()) == 0) {
            func_020113f8(func_02008444());
            if (p->count == 1) {
                if (func_02033100(func_02008444()) != 0) {
                    return;
                }
                func_0200c82c(func_02008860(), 1);
                func_0201135c(func_02008444());
                p->count = 0;
                if (p->lock != 0) {
                    return;
                }
                if (func_0201132c(func_02008444()) != 0) {
                    func_02010e7c(func_02008444());
                } else {
                    func_02010e5c(func_02008444());
                }
            } else {
                p->count = p->count - 1;
                return;
            }
        } else if (p->mode != 0) {
            if (p->mode == 1) {
                func_02010ebc(func_02008444());
                p->mode = 2;
                break;
            } else if (p->mode == 2 && func_02010b38(func_02008444()) == 0) {
                if ((data_020c9d50.trig & 0x1000) != 0) {
                    func_0202a0c0(func_0200847c(), 2);
                    func_0202a1b0(func_0200847c(), 0x20, 0);
                    func_02010ef4(func_02008444(), 10, -1);
                    func_02022f34(func_02031b20(p), data_02099ed8, 0);
                    func_02011598(func_02008444());
                    func_0201192c(func_02008444(), 0x15);
                    p->mode = 0;
                } else if ((data_020c9d50.trig & 0x2000) != 0) {
                    func_0202a0c0(func_0200847c(), 3);
                    func_0202a1b0(func_0200847c(), 0x20, 0);
                    func_02010ef4(func_02008444(), 11, -1);
                    func_0201135c(func_02008444());
                    p->mode = 0;
                    if (func_0201132c(func_02008444()) != 0) {
                        func_02010ef4(func_02008444(), 11, 3);
                    } else {
                        func_02010ef4(func_02008444(), 11, 2);
                    }
                    if (p->actor != 0) {
                        func_02061230(p->actor);
                    }
                }
            }
            return;
        } else {
            if (func_02033408(p) != 0) {
                if (func_02012518(func_02008444(), data_02099dd4, data_02099eec) != 0 && func_02012518(func_02008444(), data_02099ef8, data_02099f00) != 0) {
                    func_02022f34(func_02031b20(p), data_02099ed8, 0);
                    func_02011598(func_02008444());
                    func_0201192c(func_02008444(), 0x15);
                    return;
                }
                func_0202a0c0(func_0200847c(), 0x20);
                func_02010ef4(func_02008444(), 0xff, -1);
                p->mode = 1;
                return;
            }
            if (func_020333e4(p) != 0) {
                p->actor->a11();
                func_02061138(p->actor, 0);
                func_02033110(p->owner);
                func_0201192c(func_02008444(), 0x14);
                func_02010ef4(func_02008444(), 0xff, -1);
                func_02011598(func_02008444());
                if (func_02012518(func_02008444(), data_02099e90, data_02099f08) != 0) {
                    func_0200f958(func_02008444(), 0x4000);
                }
                p->opened = 0;
                return;
            }
            if (p->seen != 0) {
                if (func_02033124(p->actor) == 0) {
                    p->seen = 0;
                    if (func_0201132c(func_02008444()) != 0) {
                        func_02010e7c(func_02008444());
                    } else {
                        func_02010e5c(func_02008444());
                    }
                }
            } else if (func_02033124(p->actor) != 0) {
                p->seen = 1;
                func_02010ef4(func_02008444(), 0xff, -1);
            } else if (func_02010b38(func_02008444()) == 0 && func_02061074(p->actor) == 0 && p->lock == 0) {
                if ((data_020c9d50.trig & 0xa000) != 0 && func_02010fbc(func_02008444(), 2) != 0) {
                    func_02061138(p->actor, 0);
                    func_02022f2c(func_02031b20(p), data_020cd7d0, 0);
                    func_0201192c(func_02008444(), 3);
                    func_02010ef4(func_02008444(), 2, -1);
                    return;
                }
                if ((data_020c9d50.trig & 0x20000) != 0 && func_02010fbc(func_02008444(), 1) != 0 && *(short *)(data_020c9e88 + 0x12) == 0) {
                    if (func_0201132c(func_02008444()) != 0) {
                        func_02022f34(func_02031b20(p), data_02099f10, 0);
                    } else {
                        func_02022f34(func_02031b20(p), data_02099f28, 0);
                    }
                    func_02011598(func_02008444());
                    func_0201192c(func_02008444(), 0x13);
                    func_02010ef4(func_02008444(), 1, -1);
                    return;
                }
                if ((data_020c9d50.trig & 0x1000000) != 0 && func_02010fbc(func_02008444(), 7) != 0) {
                    func_0201192c(func_02008444(), 4);
                    func_02014260(func_02008444());
                    func_02022f34(func_0201123c(func_02011264(), 0), data_02099f3c, 1);
                    func_02010ef4(func_02008444(), 7, -1);
                    p->busy = 1;
                    func_02011598(func_02008444());
                    func_02033138(p->owner);
                    func_0202a0c0(func_0200847c(), 0x1f);
                    return;
                }
                if ((data_020c9d50.trig & 0x1000000) != 0 && func_02010fbc(func_02008444(), 6) != 0) {
                    func_0201192c(func_02008444(), 0x16);
                    func_0201424c(func_02008444());
                    func_02022f34(func_0201123c(func_02011264(), 0), data_02099f48, 1);
                    func_02010ef4(func_02008444(), 6, -1);
                    p->busy = 1;
                    func_02011598(func_02008444());
                    func_02033138(p->owner);
                    func_0202a0c0(func_0200847c(), 0x1f);
                }
            }
        }
        break;
    }
}
}
