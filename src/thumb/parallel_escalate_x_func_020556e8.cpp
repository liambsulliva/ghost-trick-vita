class Item {
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
    virtual void slot33();
    virtual void slot34();
    virtual void slot35(signed char value);
    unsigned char pad4[0xc4];
};

struct Stage {
    unsigned char pad0[0x28];
    Item *items;
    unsigned char pad2c[0x60];
    unsigned short idx;
    unsigned short count;
    unsigned char pad90[8];
    int speed;
};

extern "C" {
void func_020556e8(struct Stage *p, int level)
{
    unsigned short i;

    for (i = 0; i < p->count; i++) {
        p->items[i].slot6();
        if (p->speed < 0 || i == p->idx) {
            p->items[i].slot35(level);
        }
    }
}
}
