struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

class Cursor {
public:
    virtual void c0();
    virtual void c1();
    virtual void c2();
    virtual void c3();
    virtual void c4();
    virtual void c5();
    virtual void c6();
    virtual void c7();
    virtual void c8();
    virtual void c9();
    virtual void c10();
    virtual void c11();
    virtual void c12();
    virtual void c13(int x, int y, int z);
};

struct Scene {
    char pad0[4];
    unsigned char state;
    unsigned char wait;
    unsigned char busy;
    char pad7[0x19];
    void *owner;
    Cursor *cursor;
    int id;
    unsigned char moving;
    char pad2d;
    short px;
    short py;
    char pad32[2];
    int vx;
    int vy;
    char pad3c[2];
    unsigned char lock;
};

extern "C" {
extern struct Pad data_020c9d50;
extern unsigned char data_020c9e88[];
extern char data_0209a374[];
extern char data_0209a388[];
extern char data_0209a38c[];
extern char data_0209a394[];
extern char data_0209a3a4[];
extern char data_0209a3ac[];
extern char data_0209a3c0[];
extern char data_0209a3c8[];
extern char data_0209a3d8[];
extern char data_0209a3e8[];
extern char data_0209a400[];
extern void *func_02008444(void);
extern void *func_0200847c(void);
extern int func_02031af4(void *game, int id);
extern void func_020356e0(void *game, int id);
extern int func_02011320(void *game);
extern void func_0200f918(void *game, int value);
extern int func_02031b20(struct Scene *p);
extern void func_02022f34(int object, char *table, int index);
extern int func_02011244(void *game);
extern int func_020322d4(void *game);
extern void func_02010f0c(void *game, int id, int value);
extern void func_0201135c(void *game);
extern int func_0200f330(void *game);
extern int func_02010b38(void *game);
extern int func_0201132c(void *game);
extern int func_02010194(void *game);
extern int func_020100e4(int node);
extern int func_02010fbc(void *game, int id);
extern void func_02034248(void *owner, int on);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void func_02010ef4(void *game, int id, int value);
extern void func_0202682c(void *game);
extern void func_0202a0c0(void *sound, int id);
extern int func_02013a14(void *game);
extern int func_0201572c(void);
extern void func_02010384(void *game, int value);
extern void func_02011598(void *game);
extern int func_02010f84(void *game, int id);
extern void func_02014260(void *game);
extern void func_0201424c(void *game);
extern int func_020356f0(void *game);
extern int func_020357a4(struct Scene *p);
extern void *func_02010b7c(void);
extern int func_02010b60(void *object);
extern int func_02010b64(void *object);
extern int func_0202cb64(void *object);
extern int func_02035700(int value);
extern void func_020330e0(void *game, int value);
extern void func_020330c0(void *game, int value);
extern void func_02035760(struct Scene *p);
extern void func_0203571c(struct Scene *p);
extern int func_0206f28c(int a, int b);
extern void func_0200f9e8(void *game, int value);
extern int func_020267a0(void *game);
extern void func_02011690(void *game, int value);
extern int func_02012518(void *game, char *a, char *b);
extern void func_0200f958(void *game, int value);
extern void func_02035708(void *game);
extern void func_020112d4(void *game);
extern void func_0201192c(void *game, int value);
extern int func_02011264(void);
extern int func_0201123c(int object, int value);
extern void func_020322b4(void *game);

void func_02035078(struct Scene *p)
{
    int node;
    short x;
    short y;

    if (p->id >= 0 && func_02031af4(func_02008444(), p->id) != 0) {
        p->cursor = 0;
        func_020356e0(func_02008444(), p->id);
        p->id = -1;
    }
    if (func_02011320(func_02008444()) == 2) {
        func_0200f918(func_02008444(), 1);
        func_02022f34(func_02031b20(p), data_0209a374, 0);
        return;
    }
    if (p->state != 3 && p->state != 5) {
        if (func_02011244(func_02008444()) != 0 && func_020322d4(func_02008444()) != 0) {
            func_02010f0c(func_02008444(), 1, 0);
        } else {
            func_02010f0c(func_02008444(), 1, 1);
        }
    }
    if (p->busy != 0) {
        func_0201135c(func_02008444());
        p->busy = 0;
    }
    switch (p->state) {
    case 0:
        if ((p->lock == 0 && func_0200f330(func_02008444()) != 0) || func_02010b38(func_02008444()) != 0) {
            return;
        }
        if (func_0201132c(func_02008444()) == 0) {
            node = func_02010194(func_02008444());
            if (node == 0 || func_020100e4(node) != 0) {
                func_02010f0c(func_02008444(), 4, 0);
            } else {
                func_02010f0c(func_02008444(), 4, 1);
            }
        }
        if (p->lock == 0) {
            if ((data_020c9d50.trig & 0x4000) != 0 && func_02010fbc(func_02008444(), 4) != 0 && func_0201132c(func_02008444()) == 0) {
                func_02034248(p->owner, 1);
                func_0200da04(data_0209a388, data_0209a38c, data_0209a394, data_0209a3a4);
                func_02010ef4(func_02008444(), 4, -1);
                func_0202682c(func_02008444());
                *(int *)&p->state = 1;
                return;
            }
            if ((data_020c9d50.trig & 0x400000) != 0 && func_02010fbc(func_02008444(), 5) != 0) {
                func_02010ef4(func_02008444(), 5, -1);
                func_0202a0c0(func_0200847c(), 0x11);
                return;
            }
            if ((data_020c9d50.trig & 0x20000) != 0 && func_020322d4(func_02008444()) != 0 && func_02013a14(func_02008444()) != 0 && *(short *)(data_020c9e88 + 0x12) == 0 && func_0201572c() == 0) {
                func_02010ef4(func_02008444(), 1, -1);
                func_02010384(func_02008444(), 1);
                p->state = 3;
                return;
            }
            if ((data_020c9d50.trig & 0x8000) != 0 && func_02010fbc(func_02008444(), 0) != 0) {
                func_02010384(func_02008444(), 1);
                *(int *)&p->state = 4;
                return;
            }
            if ((data_020c9d50.trig & 0x2000) != 0) {
                func_0202682c(func_02008444());
                p->state = 2;
                return;
            }
            if ((data_020c9d50.trig & 0x10000) != 0 && func_02010fbc(func_02008444(), 3) != 0) {
                func_02010384(func_02008444(), 1);
                func_0202682c(func_02008444());
                func_02011598(func_02008444());
                func_02022f34(func_02031b20(p), data_0209a3ac, 0);
                func_02010ef4(func_02008444(), 3, -1);
                func_0202a0c0(func_0200847c(), 8);
                p->state = 5;
                return;
            }
            if ((data_020c9d50.trig & 0x1000000) != 0 && func_02010f84(func_02008444(), 6) != 0 && func_02010f84(func_02008444(), 7) != 0 && func_02010b38(func_02008444()) == 0) {
                func_0202682c(func_02008444());
                p->state = 2;
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
            }
        }
        if (p->lock == 0 && func_020356f0(func_02008444()) != 0) {
            return;
        }
        if (func_020357a4(p) != 0) {
            x = func_02010b60(func_02010b7c());
            y = func_02010b64(func_02010b7c());
            if (func_0202cb64(func_02010b7c()) != 0) {
                x = p->px;
                y = p->py;
                p->moving = 0;
            } else {
                p->vx = 0;
                p->vy = 0;
                if (p->moving == 0) {
                    p->moving = 1;
                } else {
                    p->vx = (p->px - x) << 12;
                    p->vy = (p->py - y) << 12;
                    if (func_02035700(p->vx) <= 0x1000) {
                        p->vx = 0;
                    }
                    if (func_02035700(p->vy) <= 0x1000) {
                        p->vy = 0;
                    }
                    p->vx = (p->vx * 0xfff) >> 12;
                    p->vy = (p->vy * 0xfff) >> 12;
                    func_020330e0(func_02008444(), p->vx);
                    func_020330c0(func_02008444(), p->vy);
                }
            }
            p->px = x;
            p->py = y;
            func_02035760(p);
            p->cursor->c13((x - 0x80) << 12, (0x60 - y) << 12, 0);
            return;
        }
        if ((data_020c9d50.cur & 0xf00) != 0) {
            if ((data_020c9d50.cur & 0x800) != 0) {
                func_020330e0(func_02008444(), 0x4000);
            } else if ((data_020c9d50.cur & 0x400) != 0) {
                func_020330e0(func_02008444(), -0x4000);
            }
            if ((data_020c9d50.cur & 0x100) != 0) {
                func_020330c0(func_02008444(), -0x4000);
            } else if ((data_020c9d50.cur & 0x200) != 0) {
                func_020330c0(func_02008444(), 0x4000);
            }
            p->moving = 0;
            p->vx = 0;
            p->vy = 0;
            func_0203571c(p);
            return;
        }
        if (p->moving != 0 && func_02035700(p->vx) < 0x3000 && func_02035700(p->vy) < 0x3000) {
            p->vy = 0;
            p->vx = 0;
        }
        p->moving = 0;
        if (func_02035700(p->vx) > 0x3000) {
            p->vx = func_0206f28c(p->vx, 0xea0);
            func_020330e0(func_02008444(), p->vx);
        } else {
            p->vx = 0;
        }
        if (func_02035700(p->vy) > 0x3000) {
            p->vy = func_0206f28c(p->vy, 0xea0);
            func_020330c0(func_02008444(), p->vy);
        } else {
            p->vy = 0;
        }
        func_0203571c(p);
        return;
    case 2:
        if (p->wait == 0) {
            p->wait = 1;
            func_0200f9e8(func_02008444(), 0x10000);
            return;
        }
        if (func_020267a0(func_02008444()) != 0) {
            return;
        }
        func_02011690(func_02008444(), 0);
        p->state = 0x63;
        return;
    case 1:
        if (p->wait == 0) {
            p->wait = 1;
            if (func_02012518(func_02008444(), data_0209a3c0, data_0209a3c8) == 0) {
                func_0200f9e8(func_02008444(), 0x10000);
                return;
            }
            func_0200da04(data_0209a388, data_0209a3c0, data_0209a3d8, data_0209a3a4);
            return;
        }
        if (func_020267a0(func_02008444()) != 0) {
            return;
        }
        p->state = 0x63;
        return;
    case 4:
        func_0200f958(func_02008444(), 0x1000000);
        func_02035708(func_02008444());
        func_02010ef4(func_02008444(), 0, -1);
        func_020112d4(func_02008444());
        func_02011598(func_02008444());
        p->state = 0xff;
        return;
    case 3:
        func_0201192c(func_02008444(), 0x13);
        func_02010384(func_02008444(), 1);
        func_02011598(func_02008444());
        func_02022f34(func_0201123c(func_02011264(), 0), data_0209a3e8, 0);
        return;
    case 5:
    case 0xff:
        break;
    default:
        if (func_0201132c(func_02008444()) == 0) {
            func_02010f0c(func_02008444(), 4, 1);
        }
        if (func_020267a0(func_02008444()) != 0) {
            return;
        }
        func_02022f34(func_02031b20(p), data_0209a400, 0);
        func_020322b4(func_02008444());
        return;
    }
}
}
