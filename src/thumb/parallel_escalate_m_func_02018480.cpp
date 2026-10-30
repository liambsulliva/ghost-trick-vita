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

struct State {
    unsigned char pad[0x12];
    unsigned short timer;
};
struct Owner {
    Actor actor;
};
extern "C" {
extern unsigned int data_020c9d50;
extern State data_020c9e88;
char *func_02008444(void);
void func_02011d04(char *object, int kind, int value);

int func_02018480(Owner *owner)
{
    if (owner->actor.dead() == 0) {
        if (owner->actor.busy() == 0 || (data_020c9d50 & 0x200000) == 0) {
            func_02011d04(func_02008444(), 4, 0);
        } else {
            data_020c9e88.timer = 0;
        }
    }
    return 0;
}
}
