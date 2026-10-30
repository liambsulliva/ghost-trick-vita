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
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual int dead();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void setTime(int time);
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void setLayer(int layer);
};

struct Info {
    short kind;
    short x;
};
struct Owner {
    Actor actor;
    unsigned char pad[0x14 - 4];
    Info *info;
};
extern "C" {
extern char data_020cf75c[];
Actor *func_02056964(char *name);
void func_0201862c(Actor *actor);
void func_02056d68(Actor *actor, int x, int y);
void func_0200ee34(Actor *actor, int r, int g, int b);
void func_02018640(Actor *actor);
void *func_02009900(void);
void func_0202f7e8(void *manager, Actor *actor, int kind, int flag);

int func_020185a0(Owner *owner)
{
    Actor *actor;
    if (owner->actor.dead() == 0) {
        actor = func_02056964(data_020cf75c);
        if (actor != 0) {
            func_0201862c(actor);
            func_02056d68(actor, 0, 0x1f);
            actor->setTime(0x560);
            func_0202f7e8(func_02009900(), actor, 0xd, 0);
        }
        actor = func_02056964(data_020cf75c);
        if (actor != 0) {
            func_0200ee34(actor, 0x1f, 0x1f, 0x1f);
            func_02018640(actor);
            func_02056d68(actor, owner->info->x, 0);
            actor->setLayer(0x37);
            func_0202f7e8(func_02009900(), actor, 0xd, 0);
        }
    }
    return 0;
}
}
