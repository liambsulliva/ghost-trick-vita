struct Info {
    unsigned short kind;
    unsigned short x;
    unsigned short y;
    unsigned short code;
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
    virtual void select(int id);
};

struct Owner {
    Actor actor;
    unsigned char pad[0x14 - 4];
    Info *info;
};
extern "C" {
extern unsigned int data_020c9d50;
char *func_02008444(void);
void func_020180d8(char *object, int x, int y, int flag);

int func_02018078(Owner *owner)
{
    int flag;
    Info *info;
    owner->actor.select(owner->info->x);
    if (owner->actor.dead() == 0) {
        flag = 0;
        if (((data_020c9d50 & 0x200000) == 0 || owner->actor.busy() == 0) && owner->info->code == 0xff0d) {
            flag = 1;
        }
        info = owner->info;
        func_020180d8(func_02008444(), info->x, info->y, flag);
    }
    return 1;
}
}
