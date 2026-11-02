typedef struct {
    int x;
    int y;
    int z;
} Vec;

class Model {
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
    virtual void slot33();
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
};

class Effect {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(Effect *self);
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
    virtual void slot33();
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53();
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60();
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65();
    virtual void slot66();
    unsigned char pad4[0x30];
    int x;
    int y;
    unsigned char pad3c[0x44];
    short lift;
    unsigned char pad82[0xa];
    unsigned char fade_in;
    unsigned char pad8d;
    short fade_len;
    short fade_pos;
    unsigned char pad92[0x2];
    int from_x;
    int from_y;
    int to_x;
    int to_y;
    short move_len;
    short move_pos;
    unsigned char pada8[0x8];
    void *anchor;
    unsigned char padb4[0x14];
    Model *model;
    unsigned char done;
    unsigned char padcd;
    unsigned short frame;
};

extern "C" {
extern int func_0202ff58(Effect *p);
extern void func_02066af0(Model *model, int frame);
extern void func_020664e0(Model *model, int r, int g, int b, int alpha);
extern int func_0206ef28(int a, int b);
extern int func_0206f28c(int a, int b);
extern unsigned int func_02066260(Effect *p, int kind);
extern void *func_02066280(Effect *p, int kind, int index);
extern void func_0205958c(Vec *v, void *node, int a);
extern int func_020083a0(void *node);
extern void func_020082d8(Vec *dst, int m);
extern void func_02008378(Vec *dst, Vec *a, Vec *b);
extern void *func_0200847c(void);
extern void *func_0202a114(void *system, void *item, int x, int y);
extern void func_02061c10(void *view, void *node, int lift);
extern void func_0202a0c0(void *system, void *item);
void func_02066928(Effect *p);
}

void func_02066928(Effect *p)
{
    int level;
    int t;
    unsigned int count;
    unsigned char i;
    void *item;
    int lift;
    void *view;
    Vec v;
    Vec m;
    Vec w;

    if (func_0202ff58(p) == 0) {
        return;
    }
    func_02066af0(p->model, p->frame);
    p->model->slot41();

    if (p->fade_len > 0) {
        if (p->fade_pos >= p->fade_len) {
            if (p->fade_in != 0) {
                p->slot66();
                p->done = 1;
            } else {
                func_020664e0(p->model, 0x1f, 0x1f, 0x1f, 0x64000);
            }
        } else {
            level = (p->fade_len - p->fade_pos) * 100 / p->fade_len;
            if (p->fade_in == 0) {
                level = 100 - level;
            }
            func_020664e0(p->model, 0x1f, 0x1f, 0x1f, level << 12);
            p->fade_pos++;
        }
    }

    if (p->move_len > 0) {
        if (p->move_pos > p->move_len) {
            p->move_pos = 0;
            p->move_len = 0;
        } else {
            t = func_0206ef28(p->move_pos << 12, p->move_len << 12);
            p->x = p->from_x + func_0206f28c(p->to_x - p->from_x, t);
            p->y = p->from_y + func_0206f28c(p->to_y - p->from_y, t);
            p->move_pos++;
        }
    }

    if (p->done != 0) {
        p->model = 0;
        p->slot8(p);
        return;
    }

    count = func_02066260(p, 2);
    for (i = 0; i < count; i++) {
        item = func_02066280(p, 2, i);
        if (p->anchor != 0) {
            lift = p->lift;
            if (lift < 0) {
                lift = 0;
            }
            func_0205958c(&v, p->anchor, lift);
            func_020082d8(&m, func_020083a0(p->anchor));
            func_02008378(&w, &v, &m);
            view = func_0202a114(func_0200847c(), item, w.x, w.y);
            if (view != 0) {
                func_02061c10(view, p->anchor, lift);
            }
        } else {
            func_0202a0c0(func_0200847c(), item);
        }
    }
}
