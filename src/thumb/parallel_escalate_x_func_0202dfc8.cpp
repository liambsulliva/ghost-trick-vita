struct Elem {
    unsigned char pad[0x6c];
};

struct Holder {
    unsigned char pad0[0x18];
    Elem *list;
};

struct Info {
    unsigned char pad0[0x184];
    unsigned short id;
};

struct Flags {
    unsigned char f0 : 1;
    unsigned char f1 : 1;
    unsigned char f2 : 1;
    unsigned char f3 : 1;
    unsigned char f4 : 1;
    unsigned char f5 : 1;
    unsigned char f6 : 1;
    unsigned char f7 : 1;
};

class Source {
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
    virtual int slot40();
};

class Actor {
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
    virtual int slot12(int a, int *b, int *c, int *d, int e);
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
    virtual void slot25(int a);
    unsigned char pad4[0x7c];
    Info *info;
    unsigned char pad84[0x2];
    Flags state;
    unsigned char pad87;
    unsigned int frame;
    unsigned char pad8c[0x4];
    int x;
    int y;
    unsigned char pad98[0xc];
    int z;
    unsigned char pada8[0x10];
    unsigned int frames;
    unsigned char padbc[0x88];
    unsigned int count;
    unsigned char pad148[0x40];
    Source *source;
    Flags flags;
    unsigned char pad18d[0x57];
    unsigned char marks[3];
    unsigned char pad1e7[0x9];
    Elem *list;
    Holder *holder;
};

extern "C" {
extern unsigned char data_020cff60[];
extern int func_0202e17c(Elem *e);
extern void func_0202e180(Elem *e, int value);
extern void func_020421d4(Elem *e, int a, int id, int t, int b);
extern void func_0202e284(Actor *p);
void func_0202dfc8(Actor *p);
}

void func_0202dfc8(Actor *p)
{
    int t;
    unsigned int i;
    unsigned int steps;
    unsigned int n;
    unsigned int j;
    Elem *list;

    t = p->source->slot40();
    for (i = 0; i < 3; i++) {
        p->marks[i] = 0;
    }
    steps = data_020cff60[1] + 1;
    if (p->holder != 0) {
        list = p->holder->list;
    } else {
        list = p->list;
    }

    for (n = 0; n < steps; n++) {
        if (p->flags.f3) {
            if (!p->flags.f4) {
                p->flags.f4 = 1;
                for (j = 0; j < p->count; j++) {
                    if (func_0202e17c(&list[j]) != 0) {
                        func_0202e180(&list[j], 1);
                    } else {
                        func_020421d4(&list[j], 0, p->info->id, t, 0);
                        p->flags.f4 = 0;
                        break;
                    }
                }
                if (!p->flags.f4) {
                    return;
                }
            }
            if (p->flags.f4) {
                for (j = 0; j < p->count; j++) {
                    func_0202e180(&list[j], 1);
                }
            }
        }

        if (p->slot12(0, &p->x, &p->y, &p->z, p->flags.f6) == 0) {
            return;
        }
        if (p->state.f3) {
            return;
        }
        if (p->frame >= p->frames && !p->state.f0 && p->frames > 1) {
            if (p->flags.f5) {
                p->frame = 0;
            } else {
                p->state.f3 = 1;
            }
        }
        if (!p->state.f3) {
            if (p->flags.f1) {
                if (p->flags.f0) {
                    p->slot25(4);
                    p->flags.f1 = 0;
                } else {
                    p->state.f3 = 1;
                }
            }
            func_0202e284(p);
        }
    }
}
