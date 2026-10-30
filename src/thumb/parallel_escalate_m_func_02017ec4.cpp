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
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual int busy();
};

struct Owner {
    Actor actor;
    unsigned char pad[0x14 - 4];
    int time;
    unsigned char pad2[0x20 - 0x18];
    unsigned short pending;
    unsigned char pad3[0x7c - 0x22];
    int start;
    unsigned char pad4[0x88 - 0x80];
    unsigned short id;
    unsigned short next;
    unsigned char locked;
};
extern "C" {
extern unsigned int data_020c9d50;
void func_0201909c(Owner *owner);
char *func_02008444(void);
void func_02017f54(char *object, int id, int flag);

int func_02017ec4(Owner *owner, unsigned int flags)
{
    unsigned short id;
    id = flags & ~0x8000;
    if ((data_020c9d50 & 0x200000) != 0 && owner->actor.busy() != 0) {
        flags &= 0x7fff;
    }
    if (owner->pending != 0) {
        func_0201909c(owner);
    }
    if ((owner->id != id && flags != 0) || owner->locked != 0) {
        if (owner->locked == 0) {
            owner->id = id;
            owner->next = 0xffff;
        }
        owner->start = owner->time;
        func_02017f54(func_02008444(), id, (flags & 0x8000) != 0);
    }
    return 1;
}
}
