struct Rec {
    unsigned char head[5];
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    int w8;
    unsigned char bc;
    unsigned char bd;
    char pade[2];
    char data[0x1ef0];
};

class Res {
public:
    virtual void r0();
    virtual void r1();
    virtual void *r2(int size, const char *name);
};

class Thing {
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
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53(int a);
};

struct Menu {
    char pad0[4];
    unsigned char state;
    char pad5;
    unsigned char timer;
    char pad7[0x39];
    struct Rec *rec;
    char pad44[4];
    unsigned short id;
    short count;
    unsigned char side;
    unsigned char flag;
    unsigned char kind;
    unsigned char mark;
    unsigned char next;
    unsigned char mode;
    unsigned char hold;
    char pad53[2];
    unsigned char exit;
    signed char fade;
    signed char wait;
    signed char delay;
    char pad59[3];
    Thing *thing;
};

extern "C" {
extern const char data_0209ab10[];
extern const char data_0209ab1c[];
extern const char data_0209ab20[];
extern void *func_020312e4(void);
extern void func_0203cbe0(void *mgr, int on);
extern int func_0203ce68(struct Menu *p, int open);
extern void func_0203d028(struct Menu *p, int open, unsigned int id, unsigned int scale, int flag);
extern int func_0203d060(struct Menu *p, int open, int a, unsigned int b, int flag);
extern void func_020521d4(const char *name, int a, unsigned int b, int c);
extern int func_0201572c(void);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *a);
extern void *func_02008444(void);
extern void *func_0203cbe4(void *object);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern int func_0201fa54(void *game);
extern int func_0203cbf0(void *object);
extern int func_0203cbf8(void *object);
extern unsigned int func_0203cbfc(void *object);
extern void func_0203cc08(void *object, int value);
extern void func_0201f33c(int *buf, Res *res, int size, char *data);
extern void func_02011994(void *game, int *buf);
extern void func_02008ca4(void *mgr, struct Rec *rec, unsigned int size, int bank);
extern void func_0201f38c(int *buf);
extern unsigned int func_0203cc14(void *mgr);
extern void func_0200fbc8(void *sys);
extern int func_02027d4c(const char *name, int mode);
extern void *func_0202f624(int size);
extern Thing *func_02067a84(void *mem);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, Thing *obj, int a, int b);
extern void func_0203cff4(struct Menu *p, int on);
extern void func_02075f84(void);
extern void *func_0200847c(void);
extern int func_020120f0(void *object, int value);
extern void func_0203ccac(struct Menu *p, unsigned int exit);
extern void func_02022f6c(struct Menu *p);

int func_0203c6a8(struct Menu *p, int a, int b, int c)
{
    unsigned char st;
    int r;
    unsigned int res;
    int sel;
    int handle;
    Thing *obj;
    short lim;
    int buf[6];

    st = p->state;
    if (st == 0 || st == 10) {
        if (p->delay > 0) {
            p->delay--;
            if (p->delay <= 0) {
                func_0203cbe0(func_020312e4(), 0);
            }
        }
        r = func_0203ce68(p, 0);
        if (r == -2) {
            return 1;
        }
        if (p->state == 0) {
            if (r == 0) {
                p->state = 5;
                func_0203cbe0(func_020312e4(), 1);
                func_0203d028(p, 0, 0x165, 0x14000, 3);
            } else if (r == 1) {
                p->state = 8;
                p->timer = 0;
            }
        } else {
            sel = -1;
            if (r == 0 && p->side == 0) {
                sel = 0;
            }
            if (r == 0 && p->side == 1) {
                sel = 1;
            }
            if (r == 1 && p->side == 0) {
                sel = 1;
            }
            if (r == 1 && p->side == 1) {
                sel = 0;
            }
            if (sel == 0) {
                if (p->mark == 1) {
                    p->side = 0;
                    p->id = 0x1a1;
                    p->state = 11;
                    func_0203d028(p, 0, 0x160, 0x14000, 0);
                } else {
                    if (a == 0) {
                        func_020521d4(data_0209ab10, 0, 6, 0x100);
                    }
                    if (c != 0) {
                        func_020521d4(data_0209ab10, 1, 6, 0x100);
                    }
                    p->state = 3;
                }
            } else if (sel == 1) {
                if (a == 0) {
                    func_020521d4(data_0209ab10, 0, 6, 0x100);
                }
                if (b != 0) {
                    func_020521d4(data_0209ab10, 1, 6, 0x100);
                }
                p->state = 1;
            }
        }
        return 1;
    } else if (st == 4) {
        p->fade++;
        if (p->fade < 0x20) {
            return 1;
        }
        p->state = 5;
        return 1;
    } else if (st == 5) {
        if (func_0201572c() != 0) {
            return 1;
        }
        if (p->flag == 0) {
            func_0203d060(p, 0, 0, 0xffff, 3);
        }
        p->rec = (struct Rec *)func_02007b70(func_02007bd8())->r2(0x1f00, data_0209ab1c);
        func_0207303c(func_0203cbe4(func_02008444()), p->rec, 5);
        p->rec->b5 = func_0201fa54(func_02008444());
        p->rec->b6 = p->kind;
        p->rec->b7 = func_0203cbf0(func_02008444());
        p->rec->w8 = func_0203cbf8(func_02008444());
        p->rec->bc = 0;
        if (p->flag != 0 || func_0203cbfc(func_02008444()) != 0) {
            p->rec->bc = 1;
            func_0203cc08(func_02008444(), 1);
        }
        p->rec->bd = 0;
        if (p->flag != 0) {
            p->rec->bd = 1;
        }
        func_0201f33c(buf, func_02007b70(func_02007bd8()), 0x1ef0, p->rec->data);
        func_02011994(func_02008444(), buf);
        func_02008ca4(func_020312e4(), p->rec, 0x1f00, 0);
        p->state = 6;
        func_0201f38c(buf);
        return 1;
    } else if (st == 6) {
        if (p->flag == 0) {
            func_0203d060(p, 0, 0, 0xffff, 3);
        }
        res = func_0203cc14(func_020312e4());
        p->wait++;
        if (p->wait > 0x3c) {
            p->wait = 0x3c;
        }
        if (res < 1 || p->wait < 0x3c) {
            return 1;
        }
        if (res == 2) {
            func_0200fbc8(func_02008444());
            handle = func_02027d4c(data_0209ab20, 2);
            obj = (Thing *)func_0202f624(0xd4);
            if (obj != 0) {
                obj = func_02067a84(obj);
            }
            p->thing = obj;
            p->thing->s53(handle);
            func_0202f7e8(func_02009900(), p->thing, 1, 0);
            p->timer = 0;
            p->state = 7;
            return 1;
        }
        func_0203cff4(p, 0);
        if (p->flag != 0) {
            func_0203d028(p, 0, 0x16f, 0, 2);
        } else {
            func_0203d028(p, 0, 0x14c, 0x14000, 2);
        }
        p->mode = 2;
        p->side = 1;
        p->mark = 0;
        p->id = 0x197;
        p->state = 9;
        return 1;
    } else if (st == 7) {
        p->timer++;
        if (p->timer > 0x10) {
            func_02075f84();
        }
        return 1;
    } else if (st == 9) {
        if (func_0203d060(p, 0, 1, p->id, p->mode) != 0) {
            if (p->flag != 0) {
                p->state = 12;
                p->exit = 2;
                p->next = 1;
                p->count = -0x80;
            } else {
                p->state = 10;
            }
            p->delay = 8;
        }
        return 1;
    } else if (st == 11) {
        if (func_0203d060(p, 0, 1, p->id, 0) != 0) {
            p->side = 0;
            p->mark = 0;
            p->state = 10;
            func_0203cff4(p, 1);
        }
        return 1;
    } else if (st == 8) {
        p->timer++;
        if (p->timer > 0x10) {
            func_0203cff4(p, 0);
            func_0203d028(p, 0, 0x16a, 0x14000, 0);
            p->mode = 0;
            p->side = 1;
            p->mark = 1;
            p->id = 0x197;
            p->state = 9;
        }
        return 1;
    } else if (st == 12) {
        if (p->flag != 0 && p->delay > 0) {
            p->delay--;
            if (p->delay <= 0) {
                func_0203cbe0(func_020312e4(), 0);
            }
        }
        if (p->hold != 0) {
            if (func_020120f0(func_0200847c(), 0) != 0) {
                return 1;
            }
            p->hold = 0;
        }
        lim = 0x10;
        p->count++;
        if (p->flag != 0) {
            lim += 0x78;
        }
        if (p->count == 10 && a != 0) {
            func_020521d4(data_0209ab10, 0, 6, 0x100);
            func_020521d4(data_0209ab10, 1, 6, 0x100);
        }
        if (p->count < lim) {
            return 1;
        }
        if (func_0201572c() != 0) {
            return 1;
        }
        if (p->exit != 0) {
            func_0203ccac(p, p->exit);
        } else {
            func_02022f6c(p);
        }
        return 1;
    }
    return 0;
}
}
