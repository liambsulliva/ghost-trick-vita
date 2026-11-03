struct Owner;
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
    virtual int slot12(Actor *parent, int *a, int *b, int *c, unsigned char arg);
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual int slot16(int x, int y);
    virtual void slot17();
    virtual void slot18();
    virtual void slot19(char *data, int a, int b, int c, int d);

    unsigned char pad4;
    unsigned char type;
    unsigned char busy;
    unsigned char pad7[9];
    unsigned short id;
    unsigned char pad12[0x62];
    Actor *child;
    Actor *next;
    int pad7c;
    Owner *owner;
    unsigned char pad84[2];
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    signed char frame;
    unsigned int index;
    int pad8c;
    int base;
    int vec[4];
    int extra;
    unsigned char pada8[0xc];
    unsigned char *done;
    unsigned int limit;
    unsigned char padbc[8];
    unsigned int count;
    int *ent_offs;
    char *ents;
    int padd0;
    int *hdr_offs;
    char *hdrs;
};

class Mgr {
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
    virtual void slot67(Actor *actor);
};

struct Owner {
    unsigned char pad[0x17c];
    unsigned short step;
    unsigned char pad17e[0xa];
    Mgr *mgr;
};

struct Ent {
    unsigned int flags;
    unsigned short id;
    unsigned char code;
    unsigned char pad7;
    int kind;
};

struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

extern "C" {
extern struct Pad data_020c9d50;
extern void *func_02010b7c(void);
extern int func_02015188(void *game);
extern int func_0202cb64(void *game);
extern int func_02010b60(void *game);
extern int func_02010b64(void *game);
int func_0202d06c(Actor *p, Actor *parent, int *a, int *b, int *c, unsigned char arg);
}

int func_0202d06c(Actor *p, Actor *parent, int *a, int *b, int *c, unsigned char arg)
{
    unsigned int i;
    Ent *ent;
    Actor *cur;
    unsigned char wrapped;
    char *data;
    int tx;
    int ty;
    Actor *node;
    Actor *sub;
    Actor *one;

    wrapped = 0;
    p->frame += (signed char)p->owner->step;
    if (p->frame >= 60) {
        wrapped = 1;
        p->frame -= 60;
    }
    if ((p->index < p->limit && !p->b0) || p->busy != 0) {
        p->slot17();
    }
    if ((p->index < p->limit && !p->b0 && !p->b1 && wrapped) || p->busy != 0) {
        p->slot18();
    }
    sub = p->child;
    while (sub != 0) {
        one = sub;
        sub = sub->next;
        one->slot12(p, &p->base, p->vec, &p->extra, arg);
    }
    for (i = 0; i < p->count; i++) {
        ent = (Ent *)(p->ents + p->ent_offs[i]);
        if (ent->kind >= 0) {
            data = p->hdrs + p->hdr_offs[ent->kind] + 0xc;
            if (*(unsigned int *)(p->hdrs + p->hdr_offs[ent->kind]) <= p->index) {
                node = p->child;
                while (node != 0) {
                    cur = node;
                    node = cur->next;
                    if (cur->type == 0x27 && cur->id == ent->id) {
                        if ((ent->flags & 0x800) != 0 && func_02015188(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
                            tx = func_02010b60(func_02010b7c());
                            ty = func_02010b64(func_02010b7c());
                            if (cur->slot16(tx, ty) != 0) {
                                cur->slot19(data, 0, 0, 0, 0);
                            }
                        }
                        if ((ent->flags & 0x20000) != 0 &&
                            (((data_020c9d50.trig & 0x1000) != 0 && ent->code == 0x61) ||
                             ((data_020c9d50.trig & 0x2000) != 0 && ent->code == 0x62) ||
                             ((data_020c9d50.trig & 4) != 0 && ent->code == 0x78) ||
                             ((data_020c9d50.trig & 8) != 0 && ent->code == 0x79) ||
                             ((data_020c9d50.trig & 0x10) != 0 && ent->code == 0x6c) ||
                             ((data_020c9d50.trig & 0x20) != 0 && ent->code == 0x72) ||
                             ((data_020c9d50.held & 0x100) != 0 && ent->code == 0xe) ||
                             ((data_020c9d50.held & 0x200) != 0 && ent->code == 0xf) ||
                             ((data_020c9d50.held & 0x400) != 0 && ent->code == 1) ||
                             ((data_020c9d50.held & 0x800) != 0 && ent->code == 2))) {
                            cur->slot19(data, 0, 0, 0, 0);
                        }
                        if ((ent->flags & 1) != 0 && !cur->b2) {
                            cur->b2 = 1;
                            cur->slot19(data, 0, 0, 0, 0);
                        }
                        if ((ent->flags & 2) != 0) {
                            cur->slot19(data, 0, 0, 0, 0);
                        }
                    }
                }
            }
        }
    }
    if (p->b1) {
        p->b1 = 0;
    } else if (!p->b0 && wrapped) {
        if (arg != 0) {
            if (p->index != 0) {
                p->index--;
            }
        } else {
            p->index++;
        }
    }
    if (p->b3) {
        if (p->done != 0) {
            *p->done = 1;
        }
        if (p != 0) {
            p->slot1();
        }
        return 0;
    }
    p->owner->mgr->slot67(p);
    return 1;
}
