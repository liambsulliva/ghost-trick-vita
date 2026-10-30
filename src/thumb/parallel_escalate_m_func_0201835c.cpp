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
    virtual void finish();
};

struct Info {
    unsigned short kind;
    unsigned short x;
};
struct Owner {
    Actor actor;
    unsigned char pad[0x14 - 4];
    Info *info;
    unsigned char pad2[0x2e - 0x18];
    unsigned short x;
    unsigned char pad3[0x80 - 0x30];
    int link;
    unsigned char pad4[0x8f - 0x84];
    unsigned char flag;
    unsigned char pad5[0x9e - 0x90];
    unsigned char mark;
    unsigned char pad6;
    unsigned short code;
    unsigned char pad7[0xa9 - 0xa2];
    unsigned char started;
};
extern "C" {
extern char data_02084cfc[];
void func_02015f3c(Owner *owner, int value);
int func_02017e78(Owner *owner);
void *func_0201c270(char *table, int code);
void func_020183d0(void *object, int value);

int func_0201835c(Owner *owner)
{
    void *found;
    if (owner->started == 0) {
        owner->x = owner->info->x;
        func_02015f3c(owner, 1);
        owner->started = 1;
        return 1;
    }
    func_02015f3c(owner, 0);
    owner->flag = 1;
    if (func_02017e78(owner) != 0) {
        found = func_0201c270(data_02084cfc, owner->code);
        if (owner->link != 0 && found != 0) {
            func_020183d0(found, owner->link);
        }
        owner->mark = 0;
    }
    owner->actor.finish();
    return 1;
}
}
