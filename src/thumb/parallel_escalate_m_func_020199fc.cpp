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

struct Info {
    unsigned short kind;
    unsigned short x;
};
struct Owner {
    Actor actor;
    unsigned char pad[0x14 - 4];
    Info *info;
    unsigned char pad2[0x6c - 0x18];
    void *part;
    unsigned char state;
};
extern "C" {
void func_0201e0c4(void *part, int state, int x);
void func_02017310(Owner *owner);

int func_020199fc(Owner *owner)
{
    func_0201e0c4(owner->part, owner->state, owner->info->x);
    if (owner->state < 2) {
        return owner->actor.finish();
    }
    func_02017310(owner);
    return 0;
}
}
