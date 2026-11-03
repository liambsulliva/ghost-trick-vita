struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

class Item {
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
    virtual void s13();
    virtual void s14();
    virtual void s15();
};

struct Box {
    char pad0[4];
    unsigned char state;
    char pad5[0x33];
    void *view;
    void *menu;
    void *left;
    void *right;
    Item *itemA;
    Item *itemB;
    int flag;
    unsigned char id;
    unsigned char done;
    unsigned char hit;
    unsigned char ready;
    unsigned char open;
    unsigned char close;
    char pad5a;
    unsigned char delay;
    unsigned char count;
    unsigned char timer;
    unsigned char wait;
};

extern "C" {
extern struct Pad data_020c9d50;
extern unsigned char data_020cdd0c[];
extern char data_0209a85c[];
extern void *func_02008444(void);
extern void *func_0200847c(void);
extern void *func_02008860(void);
extern int func_02018db4(void *game);
extern void func_0201134c(void *game);
extern void func_02011598(void *game);
extern int func_02027518(int id);
extern int func_02011320(void *game);
extern void func_02011080(void *game, int mode);
extern void func_020373f8(struct Box *p, int state, int a, int b);
extern void func_02037ac8(struct Box *p);
extern void func_0203844c(void *view);
extern void func_020386a4(void *view);
extern void func_02038090(struct Box *p, int on);
extern int func_02038524(void *view);
extern void func_02037e54(struct Box *p);
extern void func_02038670(void *view);
extern void func_0200c82c(void *sys, int a);
extern void func_02038578(void *view);
extern int func_020385b4(void *view);
extern void func_02038618(void *view);
extern int func_02038634(void *view);
extern void func_020384e8(void *view);
extern void func_02037ee0(struct Box *p);
extern void func_02037d6c(struct Box *p);
extern void func_02037ce0(struct Box *p);
extern void func_02037d8c(struct Box *p, int value);
extern int func_02031b20(struct Box *p);
extern void func_02022f34(int object, char *table, int index);
extern void func_02011340(void *game);
extern void func_0201135c(void *game);
extern int func_02022dfc(struct Box *p);
extern void func_02022f4c(int object);
extern void func_02037f20(struct Box *p);
extern void func_020384c8(void *view);
extern void func_0202a068(void *sound, int value);
extern void func_0202a16c(void *sound, int value);
extern void *func_02010b7c(void);
extern int func_02015188(void *object);
extern int func_0202cb64(void *object);
extern int func_02015154(void *object);
extern void func_0202f134(void *object);
extern void func_0203b218(void *object);
extern int func_020322d4(void *game);
extern void func_02038838(void *view);
extern int func_020387fc(void *view);
extern int func_02038220(void *view);
extern void func_020379f0(void *menu, int id);
extern void func_020379f8(void *menu);
extern void func_0202a0c0(void *sound, int id);
extern void func_02038470(void *view);
extern int func_02038b3c(void *view);
extern void func_02037a0c(struct Box *p);
extern int func_02037a1c(void *view);
extern void func_02038494(void *view);
extern void func_02037d3c(struct Box *p);
extern void func_0203833c(void *view);
extern void func_0203882c(void *view);
extern void func_0203b1e0(void *object);
extern void func_02037aa4(int side);
extern int func_02038424(void *view);

void func_02037408(struct Box *p)
{
    unsigned int trig;

    if (func_02018db4(func_02008444()) != 0) {
        return;
    }
    if (p->flag == 1) {
        func_0201134c(func_02008444());
        func_02011598(func_02008444());
        p->flag = 0;
    }
    if (p->delay != 0) {
        p->delay--;
        return;
    }
    if (p->state == 0) {
        if (func_02027518(0x1022) != 0) {
            return;
        }
        if (func_02011320(func_02008444()) != 8) {
            func_02011080(func_02008444(), 8);
        }
        func_020373f8(p, 1, 0, 0);
        return;
    }
    if (p->state == 1) {
        func_02037ac8(p);
        func_020373f8(p, 2, 2, 0);
        return;
    }
    if (p->state == 2) {
        func_0203844c(p->view);
        func_020386a4(p->view);
        func_02038090(p, 1);
        if (data_020cdd0c[1] == 0 && (p->ready == 0 || func_02038524(p->view) == 0)) {
            func_02037e54(p);
        }
        if (data_020cdd0c[1] != 0) {
            func_02038670(p->view);
        }
        func_020373f8(p, 3, 0x2d, 0);
        return;
    }
    if (p->state == 3) {
        func_0200c82c(func_02008860(), 1);
        if (data_020cdd0c[1] != 0 || (p->ready != 0 && func_02038524(p->view) != 0)) {
            p->itemA->s15();
            p->itemB->s15();
            if (data_020cdd0c[1] != 0) {
                func_020373f8(p, 5, 0, 0);
            } else {
                func_020373f8(p, 4, 0, 0);
            }
            p->timer = 0x1e;
            return;
        }
        func_020373f8(p, 8, 0, 8);
        return;
    }
    if (p->state == 4) {
        if (p->timer != 0) {
            p->timer--;
            if (p->timer != 0) {
                return;
            }
            func_02038578(p->view);
            return;
        }
        if (func_020385b4(p->view) == 0) {
            return;
        }
        if (func_02038524(p->view) != 0) {
            func_02038578(p->view);
            return;
        }
        func_020373f8(p, 6, 0, 0);
    }
    if (p->state == 5) {
        if (p->timer != 0) {
            p->timer--;
            if (p->timer != 0) {
                return;
            }
            func_02038618(p->view);
            return;
        }
        if (func_02038634(p->view) == 0) {
            return;
        }
        func_020373f8(p, 6, 0, 0);
    }
    if (p->state == 6) {
        if (data_020cdd0c[0] == 0) {
            func_020373f8(p, 0xb, 0x14, 0);
            return;
        }
        func_02037e54(p);
        func_020373f8(p, 8, 0x1e, 8);
        return;
    }
    if (p->state == 0xb) {
        func_020384e8(p->view);
        func_02037ee0(p);
        func_02037d6c(p);
        func_020373f8(p, 0xc, 0xc, 0);
        return;
    }
    if (p->state == 0xc) {
        func_02037ce0(p);
        func_020373f8(p, 0xd, 4, 0);
        if (p->done == 0) {
            return;
        }
        func_02037d8c(p, 0x1e);
        p->delay = 0x1e;
        return;
    }
    if (p->state == 0xd) {
        if (p->done == 0) {
            func_02011080(func_02008444(), p->id);
        }
        func_02022f34(func_02031b20(p), data_0209a85c, 0);
        if (func_02011320(func_02008444()) == 3) {
            func_0200c82c(func_02008860(), 1);
            func_02011340(func_02008444());
            func_0201135c(func_02008444());
            return;
        }
        func_02022f4c(func_02022dfc(p));
        return;
    }
    if (p->state == 0xe) {
        func_02037f20(p);
        return;
    }
    if (p->state == 0xa) {
        func_020384c8(p->view);
        func_020373f8(p, 0xc, 0x20, 0);
        func_0202a068(func_0200847c(), 0x3c);
        func_0202a16c(func_0200847c(), 0);
        p->done = 1;
        return;
    }
    if (func_02015188(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
        if (p->state == 7) {
            func_0202f134(func_02010b7c());
        }
    } else if (func_02015154(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
        func_0202f134(func_02010b7c());
    }
    if ((data_020c9d50.cur & 0x200) != 0) {
        func_0203b218(p->right);
    } else if ((data_020c9d50.cur & 0x100) != 0) {
        func_0203b218(p->left);
    }
    trig = data_020c9d50.trig;
    if ((trig & 0x2000) != 0) {
        if (func_020322d4(func_02008444()) != 0 || func_02027518(0x1022) != 0 || p->menu == 0) {
            return;
        }
        func_02038090(p, 1);
        func_02038838(p->view);
        p->open = 1;
        p->wait = 4;
        return;
    }
    if (p->state == 9 || p->state == 0xf) {
        if (p->count != 0) {
            p->count--;
            if (p->count == 0 && func_020387fc(p->view) == 0) {
                func_02037d6c(p);
            }
        }
        if (func_02038220(p->view) == 0) {
            return;
        }
        func_020373f8(p, 8, 0, 8);
        return;
    }
    if (p->open != 0) {
        if (p->wait != 0) {
            p->wait--;
            return;
        }
        p->open = 0;
        func_020379f0(p->menu, 0x800b);
        func_020379f8(p->menu);
        p->hit = 1;
        func_0202a0c0(func_0200847c(), 3);
        func_020373f8(p, 0xb, 0xa, 0);
        return;
    }
    if (p->close != 0) {
        if (p->wait != 0) {
            p->wait--;
            return;
        }
        p->close = 0;
        func_02038470(p->view);
        if (func_020322d4(func_02008444()) == 0 && func_02038b3c(p->view) == 0) {
            func_02037a0c(p);
            return;
        }
        if (func_020322d4(func_02008444()) != 0 && func_02037a1c(p->view) == 1) {
            p->hit = 1;
            func_020373f8(p, 0xb, 0x14, 0);
            return;
        }
        func_02038494(p->view);
        func_02037ee0(p);
        func_02037d3c(p);
        func_020373f8(p, 0xa, 0x14, 0);
        return;
    }
    if (p->state == 8) {
        func_02038090(p, 0);
        p->state = 7;
    }
    if (p->state == 7) {
        if (p->count != 0) {
            p->count--;
            if (p->count == 0) {
                func_02037d3c(p);
            }
            if (p->count == 4) {
                func_0203833c(p->view);
            }
        }
        if ((trig & 0x200) != 0 || (trig & 0x100) != 0) {
            func_0203882c(p->view);
        }
        if ((data_020c9d50.cur & 0x200) != 0) {
            func_0203b1e0(p->right);
            func_02037aa4(2);
            return;
        }
        if ((data_020c9d50.cur & 0x100) != 0) {
            func_0203b1e0(p->left);
            func_02037aa4(1);
            return;
        }
        if ((trig & 0x1000) != 0) {
            if (func_02038424(p->view) != 0) {
                func_0202a0c0(func_0200847c(), 4);
                p->delay = 4;
                return;
            }
            func_02038090(p, 1);
            func_02038838(p->view);
            p->close = 1;
            p->wait = 4;
        }
    }
}
}
