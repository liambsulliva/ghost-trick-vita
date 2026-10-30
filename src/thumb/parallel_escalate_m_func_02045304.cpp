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
    virtual void slot33(int value);
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
    virtual void slot46(int x, int y);
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
    virtual void slot66();
    virtual void slot67(unsigned short id, int a, int b, int c, int d);
    virtual void slot68(Actor *actor, void *data, int value);
};
struct Pair {
    unsigned short v[2];
};
extern "C" {
extern struct Pair data_0209b52c;
extern char data_0209b534;
extern void func_0205765c(int param_1, int param_2, int param_3, int param_4, int param_5);
void func_02045304(Actor *actor)
{
    unsigned char *p = (unsigned char *)actor;
    struct Pair ids;
    int bits;

    func_0205765c((int)actor, 0, 0, 0x40000, 0x40000);
    actor->slot46(0x40000, 0x40000);
    actor->slot33(0x571);
    bits = p[0xe0];
    p[0xe0] = bits & ~4;
    p[0x119] = 0;
    ids = data_0209b52c;
    actor->slot67(ids.v[p[0x11b]], 6, 0x40, 0x40, 3);
    actor->slot68(actor, &data_0209b534, (signed char)p[0x10a]);
}
}
