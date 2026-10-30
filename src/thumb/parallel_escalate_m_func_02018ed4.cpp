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
    virtual void enter(void *info, int flag);
    virtual void leave(void *info, int flag);
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
    virtual int finish();
};

struct Owner {
    Actor actor;
    unsigned char pad[0x14 - 4];
    void *info;
    unsigned char pad2[0x20 - 0x18];
    unsigned short pending;
    unsigned char pad3[0x7c - 0x22];
    void *saved;
    unsigned char pad4[0x88 - 0x80];
    unsigned short id;
    unsigned short next;
    unsigned char pad5[0xa7 - 0x8c];
    unsigned char done;
};
extern "C" {
char *func_02008444(void);
void func_02017f54(char *object, int id, int flag);
void func_02011984(char *object, int id, int flag);

void func_02018ed4(Owner *owner)
{
    unsigned short id;
    unsigned short next;
    if (owner->saved != 0) {
        owner->info = owner->saved;
    }
    owner->done = 1;
    id = owner->id;
    if (id != 0) {
        func_02017f54(func_02008444(), id, 0);
        owner->actor.enter(owner->info, 0);
        return;
    }
    next = owner->next;
    if (next != 0xffff) {
        func_02011984(func_02008444(), next, 0);
        owner->actor.leave(owner->info, 0);
        return;
    }
    if (owner->pending == 0) {
        func_02017f54(func_02008444(), id, 0);
        owner->actor.enter(owner->info, 0);
        return;
    }
    func_02011984(func_02008444(), next, 0);
    owner->actor.leave(owner->info, 0);
}
}
