class Anim {
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
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
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
    virtual void slot33(int value);
    virtual void slot34();
    virtual void slot35(signed char value);
};

class Part {
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
};

class Item {
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
    unsigned char pad4[0xc4];
};

struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

struct Flags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
};

class Stage {
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
    unsigned char pad4[0x24];
    Item *items;
    Anim *anim;
    Part *part;
    unsigned char view[0x54];
    struct Flags flags;
    unsigned char state;
    unsigned char stage;
    unsigned char stages;
    unsigned short idx;
    unsigned short count;
    unsigned short timer;
    short step;
    int level;
    int speed;
    int counter;
    unsigned char opts;
    unsigned char pada1;
    unsigned short limit;
    unsigned short count2;
    unsigned char pada6[2];
    int text;
    short wait[8];
    unsigned short hold[8];
};

extern "C" {
extern struct Pad data_020c9d50;
extern char data_020cf740[];
extern char *data_0209d70c[];
extern char data_0209d71c[];
extern char data_0209d72c[];
extern char data_0209d734[];
extern char data_0209d740[];
extern char data_0209d74c[];
extern void *func_0201003c(void);
extern void *func_02015c7c(void *object);
extern int func_02055404(void *sys);
extern Anim *func_02056950(char *name);
extern void func_020153d8(Anim *kid, void *owner);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, void *obj, int a, int b);
extern void func_02055444(Stage *p, void *sys);
extern void func_02042820(void *view, int text, int x, int a, int b, int c, int d, int e);
extern int func_02042b30(void *view);
extern void func_020556e8(Stage *p, int value);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void *func_02010b7c(void);
extern int func_02015188(void *sys);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *a, int b);
void func_02054f04(Stage *p);
}

void func_02054f04(Stage *p)
{
    void *sys;
    unsigned short i;
    int done;

    sys = func_02015c7c(func_0201003c());
    if (sys == 0) {
        return;
    }
    if (p->flags.b0 == 0) {
        p->stages = func_02055404(sys);
        p->speed = 0x199a;
        p->counter = 0;
        p->count2 = 0;
        p->anim = func_02056950(data_020cf740);
        p->anim->slot33(0x530);
        func_020153d8(p->anim, p);
        func_0202f7e8(func_02009900(), p->anim, 7, 0);
        func_02055444(p, sys);
        p->flags.b0 = 1;
    }
    if (p->flags.b1 == 0) {
        func_02042820(p->view, p->text, 0x6000, 3, 2, 0, 0xffff, -1);
        if (func_02042b30(p->view) != 0) {
            *(unsigned char *)&p->flags |= 2;
            for (i = 0; i < p->count; i++) {
                p->items[i].slot10();
            }
        }
    }
    p->step = 1;
    if (p->state != 4) {
        p->level = p->level + p->speed * p->step;
        if (p->level > 0x1f000) {
            p->level = 0x1f000;
        }
        if (p->level < 0) {
            p->level = 0;
        }
    }
    switch (p->state) {
    case 0:
        p->part->slot11();
        if (p->opts & 1) {
            p->state = 2;
            p->timer = 0;
            p->anim->slot35(0x10);
            p->speed = 0;
            p->level = 0x1f000;
            p->idx = p->count;
            return;
        }
        if (p->level > 0x10000) {
            p->state = 2;
            p->timer = 0;
            p->level = 0;
            p->speed = 0x800;
            p->anim->slot35(0x10);
        } else {
            p->anim->slot35(p->level >> 12);
        }
        break;
    case 1:
        p->part->slot11();
        if (p->level == 0) {
            p->slot8(0);
            return;
        }
        p->anim->slot35(p->level >> 12);
        break;
    case 2:
        p->part->slot11();
        p->timer += p->step;
        if (p->timer > p->hold[p->idx]) {
            p->timer = p->hold[p->idx];
        } else {
            p->level = 0;
        }
        func_020556e8(p, p->level >> 12);
        if (p->level == 0x1f000) {
            p->counter = 0;
            p->count2 = 0;
            p->speed = 0;
            p->state = 4;
            if (p->idx < 4) {
                func_0200da04(data_0209d71c, data_0209d734, data_0209d70c[p->idx], data_0209d72c);
            }
        }
        break;
    case 3:
        p->counter = p->counter + p->step;
        p->part->slot11();
        func_020556e8(p, p->level >> 12);
        if (p->counter > 0x3c) {
            p->counter = 0;
            p->count2 = 0;
            if (p->stage >= p->stages - 1) {
                p->state = 1;
                p->level = 0x10000;
                p->speed = -0x199a;
            } else {
                p->stage++;
                p->idx = 0;
                p->speed = 0x800;
                p->state = 2;
                p->timer = 0;
                func_02055444(p, sys);
            }
        }
        break;
    case 4:
        done = 0;
        p->counter = p->counter + p->step;
        if (p->idx >= p->count) {
            if (p->counter >= 0x1f) {
                p->counter = 0x1f;
                if (p->limit == 0) {
                    p->part->slot10();
                }
                p->count2++;
                if (p->count2 > p->limit) {
                    p->count2 = p->limit;
                }
                if (((data_020c9d50.trig & 0x1200) != 0 || func_02015188(func_02010b7c()) != 0) && p->limit == 0) {
                    done = 1;
                } else if (p->count2 >= p->limit && p->limit != 0) {
                    done = 1;
                }
            }
        } else if (p->counter >= p->wait[p->idx]) {
            done = 1;
        }
        if (done == 0) {
            return;
        }
        p->counter = 0;
        p->count2 = 0;
        p->idx++;
        if (p->idx >= p->count + 1) {
            p->speed = -0x800;
            p->state = 3;
            if (p->limit == 0) {
                func_0202a0c0(func_0200847c(), 5);
            }
        } else if (p->idx >= p->count) {
            p->speed = 0;
        } else {
            p->speed = 0x800;
            p->level = 0;
            p->state = 2;
            p->timer = 0;
        }
        if (p->idx >= p->count + 1) {
            if (p->stage >= p->stages - 1) {
                if (p->opts & 2) {
                    p->slot8(0);
                    return;
                } else {
                    func_0200da04(data_0209d71c, data_0209d734, data_0209d740, data_0209d72c);
                }
            } else {
                func_0200da04(data_0209d71c, data_0209d734, data_0209d74c, data_0209d72c);
            }
        }
        break;
    }
}
