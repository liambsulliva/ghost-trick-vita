class Menu {
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
    virtual int s12();
};

struct Step {
    short first;
    short pad2;
    short next;
    short limit;
    short wait;
};

struct Steps {
    char pad0[4];
    short x;
    short y;
    char pad8[4];
    struct Step e[1];
};

struct MenuData {
    char pad0[0x28];
    char cursor[0x130];
    struct Steps *steps;
    short timer;
    short count;
    signed char sel;
    unsigned char state;
    unsigned char busy;
    char pad163;
    signed char kind;
};

extern "C" {
extern void *func_02008444(void);
extern void func_02010384(void *game, int value);
extern void func_0200fe38(void *game, int value);
extern void func_020322b4(void *game);
extern void func_0202682c(void *game);
extern void func_0205964c(struct MenuData *m);
extern void func_02010dc4(struct MenuData *m);
extern void func_02008418(struct MenuData *m);
extern void func_02059234(struct MenuData *m, int value);
extern void func_0202801c(struct MenuData *m, int x, int y, int low);
extern void func_020280a8(char *cursor, int frame);
extern void func_02026734(struct MenuData *m, unsigned short value);
extern void func_ov006_020d4880(struct MenuData *m);
void func_ov006_020d46a8(Menu *menu);
}

void func_ov006_020d46a8(Menu *menu)
{
    struct MenuData *m;
    signed char next;

    m = (struct MenuData *)menu;
    if (m->state == 1) {
        m->state = 2;
        func_02059234(m, 0);
        if (m->sel == 0) {
            func_0202801c(m, m->steps->x, m->steps->y, 0);
        }
        if (m->kind == 0x2b && m->busy == 1 && m->sel == 0) {
            func_020280a8(m->cursor, 5);
        }
        if (m->kind == 0x2b && m->busy == 1 && m->sel == 2) {
            func_020280a8(m->cursor, 7);
        }
    } else if (m->state == 2) {
        m->state = 0;
        next = m->steps->e[m->sel].next;
        if (next < 0) {
            next = 0;
        }
        if (next < 0x40) {
            func_02026734(m, next);
        }
    }
    func_02010384(func_02008444(), 0);
    func_0200fe38(func_02008444(), 0);
    func_020322b4(func_02008444());
    func_0205964c(m);
    if (m->count >= m->steps->e[m->sel].limit) {
        func_02010dc4(m);
    } else {
        func_02008418(m);
    }
    m->count++;
    if (m->busy != 0 && !(m->kind == 0x2b && m->busy == 1 && m->sel < 2) && !(m->kind == 0x18 && m->busy == 1 && m->sel < 3)) {
        func_02010384(func_02008444(), 1);
        func_0200fe38(func_02008444(), 1);
        func_0202682c(func_02008444());
        return;
    }
    if (menu->s12() != 0 && m->timer == 0) {
        if (m->steps->e[m->sel].wait >= 0) {
            m->timer = 1;
        } else {
            func_ov006_020d4880(m);
        }
    } else if (m->timer != 0) {
        m->timer++;
        if (m->timer > m->steps->e[m->sel].wait) {
            m->timer = 0;
            func_ov006_020d4880(m);
        }
    }
    func_02010384(func_02008444(), 1);
    func_0200fe38(func_02008444(), 1);
    func_0202682c(func_02008444());
}
