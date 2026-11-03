struct Vec {
    int x;
    int y;
    int z;
};

class Obj {
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
    virtual void slot12(struct Vec *v);
    virtual void slot13();
    virtual void slot14(int x);
    virtual void slot15(int y);
    virtual void slot16();
    virtual struct Vec *slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35(signed char value);
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42(int id, int a, int b, int c, int d, int e, int f);
    virtual void slot43();
};

class Slider {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int value);
    unsigned char pad4[0xc];
    unsigned char state;
    unsigned char pad11[0x17];
    int vel;
    int level;
    int rate;
    unsigned short id;
    unsigned short dir;
    unsigned short sound;
    unsigned short wide;
    unsigned char pad3c[2];
    unsigned char on;
    unsigned char next_on;
    unsigned char ready;
    unsigned char kill;
    unsigned short next_id;
    unsigned short next_dir;
    unsigned short mode;
    unsigned short span;
    unsigned char chime;
    unsigned char pad4b;
    unsigned short chime_id;
    unsigned char pad4e[2];
    Obj *obj;
    void *menu;
    struct Vec pos;
    int bob;
    int bob_vel;
    int bob_acc;
};

struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

extern "C" {
extern struct Pad data_020c9d50;
extern void func_020082d8(struct Vec *dst, struct Vec *src);
extern void func_020083b0(struct Vec *dst, struct Vec *src);
extern void *func_0201003c(void);
extern int func_02015aa0(void *mgr);
extern void *func_02008444(void);
extern void func_0200e62c(void *game);
extern void func_0205765c(Obj *object, int a, int b, int c, int d);
extern int func_0206ef28(int a, int b);
extern void func_02015990(void *object, int value);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *a, int b);
extern void func_020549f4(void *menu);
extern void func_020549c4(void *menu);
extern void func_02054a2c(void *menu);
extern void func_02054a1c(void *menu);
extern int func_020151b0(void *game);
extern void func_02015954(void *menu);
extern int func_020547dc(int level);
extern int func_0206f28c(int a, int b);
extern int func_020547e0(void *game);
extern int func_02049bdc(void *game);
extern int func_02049be8(void *game);
void func_020542c0(Slider *p);
}

void func_020542c0(Slider *p)
{
    struct Vec pos;
    int spd;
    int lim;
    int off;
    unsigned short k;

    p->obj->slot12(&p->pos);
    func_020082d8(&pos, p->obj->slot17());
    switch (p->state) {
    case 0:
again:
        if (p->kill != 0) {
            p->slot8(0);
            return;
        }
        if (p->next_id != 0) {
            p->id = p->next_id;
            spd = 0x10000;
            p->dir = p->next_dir;
            p->on = p->next_on;
            p->next_id = 0;
            p->next_on = 0;
            p->next_dir = 0;
            if (func_02015aa0(func_0201003c()) != 0 && (data_020c9d50.cur & 0x200000) != 0) {
                spd = 0x40000;
                func_0200e62c(func_02008444());
            }
            if (p->dir == 0) {
                pos.x = -0xc0000;
                p->vel = spd;
            } else {
                pos.x = 0xc0000;
                p->vel = -spd;
            }
            if (p->id != 0 && p->id != 0x8000) {
                p->obj->slot42(p->id, 0, 2, 0, 1, -1, 0);
                if (p->dir != 0) {
                    if (p->wide != 0) {
                        func_0205765c(p->obj, 0, 0, 0x40000, 0xc0000);
                    } else {
                        func_0205765c(p->obj, 0, 0, 0x80000, 0xc0000);
                    }
                } else {
                    if (p->wide != 0) {
                        func_0205765c(p->obj, 0x3f000, 0, 0, 0xc0000);
                    } else {
                        func_0205765c(p->obj, 0x7f000, 0, 0, 0xc0000);
                    }
                }
                p->obj->slot10();
            }
            if (p->mode != 0) {
                if (p->dir == 0) {
                    pos.x = -0x40000;
                } else {
                    pos.x = 0x40000;
                }
                if (p->mode == 1) {
                    if (p->span == 0) {
                        p->state = 2;
                        func_02015990(func_0201003c(), 0);
                        p->obj->slot35(0x1f);
                    } else {
                        p->state = 4;
                        p->rate = func_0206ef28(0x20000, p->span << 12);
                        p->level = 0;
                        p->obj->slot35(0);
                    }
                } else {
                    if (p->span == 0) {
                        p->state = 2;
                        func_02015990(func_0201003c(), 0);
                        p->obj->slot35(0);
                    } else {
                        p->state = 5;
                        p->rate = func_0206ef28(0x20000, p->span << 12);
                        p->level = 0x20000;
                        p->obj->slot35(0x1f);
                    }
                }
                p->mode = 0;
            } else if (p->on != 0) {
                if (p->dir == 0) {
                    pos.x = -0x40000;
                } else {
                    pos.x = 0x40000;
                }
                p->state = 2;
                func_02015990(func_0201003c(), 0);
                func_0202a0c0(func_0200847c(), p->sound);
                func_020549f4(p->menu);
            } else {
                p->state = 1;
                func_020549c4(p->menu);
            }
        }
        break;
    default:
        if (p->ready != 0) {
            p->ready = 0;
            p->state = 3;
            if (p->mode == 0) {
                if (p->on != 0 && p->next_on == 0) {
                    func_02054a2c(p->menu);
                } else if (p->on == 0) {
                    func_02054a1c(p->menu);
                }
            }
        }
        break;
    case 1:
        if (p->dir == 0) {
            lim = -0x40000;
            if (p->wide != 0) {
                lim = -0x60000;
            }
            if (pos.x < lim) {
                pos.x += p->vel + p->vel / 4;
                if (pos.x >= lim) {
                    pos.x = lim + 0x8000;
                }
            } else {
                pos.x = lim;
                p->state = 2;
                func_02015990(func_0201003c(), 0);
                func_0202a0c0(func_0200847c(), p->sound);
            }
        } else {
            lim = 0x40000;
            if (p->wide != 0) {
                lim = 0x60000;
            }
            if (pos.x > lim) {
                pos.x += p->vel + p->vel / 4;
                if (pos.x <= lim) {
                    pos.x = lim - 0x8000;
                }
            } else {
                pos.x = lim;
                p->state = 2;
                func_02015990(func_0201003c(), 0);
                func_0202a0c0(func_0200847c(), p->sound);
            }
        }
        break;
    case 3:
        k = p->mode;
        if (k == 2 && p->next_id == p->id) {
            if (p->span == 0) {
                p->state = 2;
                func_02015990(func_0201003c(), 0);
                p->obj->slot35(0);
            } else {
                p->state = 5;
                p->rate = func_0206ef28(0x20000, p->span << 12);
                p->level = 0x20000;
                p->obj->slot35(0x1f);
            }
            p->mode = 0;
        } else if (p->on != 0 || k == 1) {
            p->state = 0;
            p->obj->slot11();
            p->obj->slot43();
            goto again;
        } else if (p->dir == 0) {
            if (pos.x > -0xbffe0) {
                pos.x += -p->vel * 3 / 2;
            } else {
                p->state = 0;
                p->obj->slot11();
                p->obj->slot43();
            }
        } else {
            if (pos.x < 0xbffe0) {
                pos.x += -p->vel * 3 / 2;
            } else {
                p->state = 0;
                p->obj->slot11();
                p->obj->slot43();
            }
        }
        break;
    case 4:
        p->level = p->level + p->rate;
        if (p->level >= 0x1f000) {
            p->level = 0x1f000;
            p->state = 2;
            func_02015990(func_0201003c(), 0);
            if (func_020151b0(func_02008444()) != 0) {
                func_02015954(func_0201003c());
            }
            if (p->chime != 0) {
                func_0202a0c0(func_0200847c(), p->chime_id);
            }
        }
        p->obj->slot35(func_020547dc(p->level));
        break;
    case 5:
        p->level = p->level - p->rate;
        if (p->level <= 0) {
            p->level = 0;
            p->state = 2;
            func_02015990(func_0201003c(), 0);
            if (func_020151b0(func_02008444()) != 0) {
                func_02015954(func_0201003c());
            }
        }
        p->obj->slot35(func_020547dc(p->level));
        break;
    }
    func_020083b0(&p->pos, &pos);
    if (p->id == 0x119 || p->id == 0xd2) {
        p->bob = p->bob + p->bob_vel;
        p->bob_vel = p->bob_vel + p->bob_acc;
        if (p->bob_vel > 0x333) {
            p->bob_vel = 0x333;
        }
        if (p->bob_vel < -0x333) {
            p->bob_vel = -0x333;
        }
        p->bob_acc = func_0206f28c(p->bob, -4);
        pos.y += p->bob - 0xa000;
    }
    if (func_020547e0(func_02008444()) != 0) {
        off = func_02049bdc(func_02008444());
        if ((p->dir == 0 && off > 0) || (p->dir != 0 && off < 0)) {
            off = -off;
        }
        p->obj->slot14(off + pos.x);
        p->obj->slot15(func_02049be8(func_02008444()));
    } else {
        p->obj->slot12(&pos);
    }
}
