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
    unsigned char pad[0x88 - 4];
    unsigned short id;
    unsigned short next;
    unsigned char locked;
};
extern "C" {
extern unsigned int data_020c9d50;
char *func_02008444(void);
void func_02011984(char *object, int id, int flag);
void func_02019134(Owner *owner);

int func_02017f78(Owner *owner, unsigned int flags)
{
    unsigned short id;
    id = flags & ~0x8000;
    if ((data_020c9d50 & 0x200000) != 0 && owner->actor.busy() != 0) {
        flags &= 0x7fff;
    }
    if ((owner->next != id && flags != 0xffff) || owner->locked != 0) {
        if (owner->locked == 0) {
            owner->id = 0;
            owner->next = id;
        }
        func_02011984(func_02008444(), id, (flags & 0x8000) != 0);
    }
    func_02019134(owner);
    return 1;
}
}
