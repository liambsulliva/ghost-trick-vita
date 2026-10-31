class Deletable02008734 {
public:
    virtual ~Deletable02008734();
};

class Sink02008734 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void release(int handle);
};

class Thing02008734 {
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
    virtual void play(int id);
};

struct Entry02008734 {
    unsigned short f0;
    unsigned short f2;
    unsigned short f4;
    unsigned short f6;
};

struct Msg02008734 {
    unsigned short f0;
    unsigned short id;
    unsigned short anim;
};

struct Self02008734 {
    char pad0[0xc];
    unsigned short id;
    char pad_e[4];
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    char pad13;
    Thing02008734 *thing;
    int handle;
    Deletable02008734 *owned;
};

extern "C" {
extern char data_020cf0f4;
extern char data_020cf87c;
extern char data_020cf388;
void func_0200b598(void *table, int id, int kind);
Entry02008734 *func_0200bb1c(void *table, int id);
void *func_02007bd8(void);
Sink02008734 *func_02007b70(void *object);
void *func_0200883c(void);
int func_02008824(void *object);
void *func_02008860(void);
int func_0200c820(void);
int func_0201e888(int owner, int kind, int id, int zero, int extra);
Deletable02008734 *func_0201e664(int owner, int kind, int id);
int func_02008828(Thing02008734 *object, void *kind);

void func_02008734(Self02008734 *self, Msg02008734 *msg)
{
    int kind;
    Entry02008734 *entry;
    int owner;
    int extra;

    self->id = msg->id;
    if (self->b2) {
        kind = 4;
    } else {
        kind = 5;
    }
    func_0200b598((char *)self->thing + 0x28, msg->id, kind);
    entry = func_0200bb1c((char *)self->thing + 0x28, msg->id);
    if (entry != 0) {
        if (self->handle != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->release(self->handle);
                self->handle = 0;
            }
        }
        owner = func_02008824(func_0200883c());
        func_02008860();
        extra = func_0200c820();
        self->handle = func_0201e888(owner, 3, entry->f6, 0, extra);
        if (self->owned != 0) {
            delete self->owned;
            self->owned = 0;
        }
        self->owned = func_0201e664(func_02008824(func_0200883c()), 3, entry->f2);
    }
    if (msg->anim == 0) {
        if (func_02008828(self->thing, &data_020cf0f4) != 0) {
            self->thing->play(msg->anim);
            return;
        }
    }
    if (msg->anim != 0) {
        if (func_02008828(self->thing, &data_020cf87c) != 0 ||
            func_02008828(self->thing, &data_020cf388) != 0) {
            self->thing->play(msg->anim);
        }
    }
}
}
