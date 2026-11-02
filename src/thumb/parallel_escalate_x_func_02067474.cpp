typedef struct {
    int x;
    int y;
    int z;
} Vec;

class Node {
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
    virtual int slot24();
    virtual int slot25();
};

class Fader {
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
    virtual void slot12(Vec *pos);
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
    virtual void slot67();
    virtual void slot68();
    virtual void slot69();
    virtual void slot70();
    virtual void slot71();
    virtual void slot72();
    virtual void slot73();
    virtual void slot74();
    virtual void slot75(int value);
    virtual void slot76();
    virtual void slot77();
    virtual void slot78();
    virtual void slot79();
    virtual void slot80();
    virtual void slot81();
    virtual void slot82();
    virtual void slot83();
    virtual void slot84();
    virtual void slot85();
    virtual void slot86();
    virtual void slot87();
    virtual void slot88();
    virtual void slot89();
    virtual void slot90();
    virtual void slot91(int r, int g, int b, signed char level);
    unsigned char pad4[0xd2];
    short wait;
    short timer;
    short step;
    short next;
    unsigned char chained;
    unsigned char started;
    unsigned char held;
    unsigned char fading;
};

extern "C" {
extern char data_020cf898[];
extern char data_020cf2e8[];
extern int func_0202ff58(Fader *p);
extern void *func_02029640(Fader *p);
extern Node *func_020082f8(void *object, char *name);
extern int func_02010b54(void);
extern int func_020083a0(Node *node);
extern void func_0205958c(Vec *v, Node *node, int a);
extern void func_02008378(Vec *dst, int m, Vec *src);
extern void func_020660c4(Fader *p);
void func_02067474(Fader *p);
}

void func_02067474(Fader *p)
{
    Node *node;
    int m;
    int level;
    Vec w;
    Vec v;

    if (func_0202ff58(p) == 0) {
        return;
    }
    node = func_020082f8(func_02029640(p), data_020cf898);
    if (node != 0) {
        if (func_02010b54() == 0) {
            p->started = 0;
            p->slot91(0x1f, 0x1f, 0x1f, 0);
            if (node->slot24() != 0 || node->slot25() != 0) {
                p->held = 1;
            }
            return;
        }
        if (p->started == 0) {
            p->started = 1;
            if (p->held == 0) {
                p->fading = 1;
                p->step = 0;
            }
            p->held = 0;
        }
        node = func_020082f8(node, data_020cf2e8);
        m = func_020083a0(node);
        func_0205958c(&v, node, 10);
        func_02008378(&w, m, &v);
        p->slot12(&w);
    }

    if (p->fading != 0) {
        level = p->step * 0x1f / 60;
        p->step++;
        if (level > 0x1f) {
            level = 0x1f;
            p->fading = 0;
        }
        p->slot91(0x1f, 0x1f, 0x1f, level);
        func_020660c4(p);
        return;
    }

    if (p->wait > 0) {
        p->wait--;
        if (p->wait <= 0) {
            p->timer = 100;
        }
    }

    if (p->timer > 0) {
        p->timer--;
        if (p->timer <= 0) {
            if (p->chained != 0) {
                p->slot75(p->next);
            } else {
                p->slot91(0x1f, 0x1f, 0x1f, 0);
                return;
            }
        }
        if (p->chained == 0) {
            p->slot91(0x1f, 0x1f, 0x1f, p->timer * 0x1f / 100);
        }
    }

    func_020660c4(p);
}
