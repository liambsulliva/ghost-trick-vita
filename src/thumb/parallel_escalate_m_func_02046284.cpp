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
    virtual void slot15();
    virtual void slot16();
    virtual int slot17();
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
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60();
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65();
    virtual void slot66(int value);
    virtual void slot67();
    virtual void slot68(Actor *actor, void *data, int value);
};
struct Mode {
    unsigned char low : 5;
    unsigned char mode : 3;
};
extern "C" {
extern char data_0209b594;
extern int func_020109c0(Actor *actor);
extern void func_02027458(int value);
extern unsigned int *func_02008444(void);
extern void func_02010384(unsigned int *object, int value);
int func_02046284(Actor *actor)
{
    unsigned char *p = (unsigned char *)actor;
    unsigned int mode;

    if (func_020109c0(actor) != 0) {
        return 0;
    }
    mode = ((struct Mode *)(p + 0x130))->mode;
    if (mode == 3 || (((unsigned int)p[0xe0]) << 29) >> 31 != 0) {
        return 0;
    }
    if (mode != 2) {
        return 0;
    }
    actor->slot66(actor->slot17());
    func_02027458(*(int *)(p + 0x120));
    ((struct Mode *)(p + 0x130))->mode = 3;
    actor->slot68(actor, &data_0209b594, 6);
    func_02010384(func_02008444(), 1);
    return 1;
}
}
