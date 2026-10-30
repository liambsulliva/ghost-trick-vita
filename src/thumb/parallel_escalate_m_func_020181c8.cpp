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

struct Owner {
    Actor actor;
    unsigned char pad[0x8c - 4];
    unsigned char busy;
    unsigned char pad2[4];
    unsigned char toggle;
    unsigned char shown;
};
extern "C" {
extern char data_02084cdc[];
extern char data_02084ce4[];
extern char data_02084cf0[];
extern char data_02084cf4[];
int func_02018248(Owner *owner);
void *func_0201003c(void);
void func_02015990(void *object, int value);
char *func_02008444(void);
int func_02012518(char *object, char *a, char *b);
void func_0200da04(char *a, char *b, char *c, char *d);

int func_020181c8(Owner *owner)
{
    if (owner->actor.dead() == 0 && func_02018248(owner) == 0) {
        if (owner->toggle == 0) {
            func_02015990(func_0201003c(), 1);
            owner->shown = 1;
        } else {
            owner->toggle = 0;
        }
        if (owner->busy == 0) {
            if (func_02012518(func_02008444(), data_02084cdc, data_02084ce4) != 0) {
                func_0200da04(data_02084cf0, data_02084cdc, data_02084ce4, data_02084cf4);
            }
            return 1;
        }
    }
    return 0;
}
}
