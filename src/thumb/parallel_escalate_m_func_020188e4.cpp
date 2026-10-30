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

struct Owner {
    Actor actor;
    unsigned char pad[0x2c - 4];
    unsigned char active;
    unsigned char pad2;
    unsigned short speed;
    unsigned char pad3[0xa4 - 0x30];
    unsigned char ready;
};
extern "C" {
extern unsigned int data_020c9d50;
int func_020164c4(Owner *owner);

void func_020188e4(Owner *owner)
{
    if (owner->ready != 0 && func_020164c4(owner) != 0 && owner->actor.busy() != 0 && (data_020c9d50 & 0x200000) != 0) {
        if (owner->speed > 1) {
            owner->speed = 1;
        }
        if (owner->active != 0) {
            owner->active = 0;
        }
    }
}
}
