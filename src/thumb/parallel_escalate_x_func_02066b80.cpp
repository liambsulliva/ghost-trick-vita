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
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53(int value);
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
    virtual void slot67();
    virtual void slot68();
    virtual void slot69();
    virtual void slot70();
    virtual void slot71();
    virtual void slot72();
    virtual void slot73();
    virtual void slot74();
    virtual void slot75();
    virtual void slot76();
    virtual void slot77();
    virtual void slot78();
    virtual void slot79();
    virtual void slot80();
    virtual void slot81();
    virtual void slot82();
    virtual void slot83();
    virtual void slot84();
    virtual void slot85();
    virtual void slot86();
    virtual void slot87();
    virtual void slot88(int value);

    unsigned char pad4[0x84];
    unsigned short x88;
    unsigned char x8a;
    unsigned char pad8b[0x39];
    int xc4;
    unsigned char padc8[0xc];
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char padd5;
    unsigned short id;
};

extern "C" {
extern void func_02066074(Actor *p, int value);
void func_02066b80(Actor *p, unsigned short id);
}

void func_02066b80(Actor *p, unsigned short id)
{
    p->id = id;
    func_02066074(p, p->x88);
    p->slot53(0);
    switch (p->id) {
    case 0x2a:
    case 0x2b:
    case 0x3c:
        p->slot53(1);
        if (p->id == 0x2a) {
            p->slot33(0x5e0);
        } else {
            p->slot33(0x600);
        }
        p->b0 = 0;
        break;
    case 0xf0:
        p->slot53(1);
        p->slot33(0x620);
        p->b0 = 0;
        break;
    case 0xe:
        p->slot53(1);
        p->slot33(0x480);
        p->b0 = 0;
        break;
    case 0x3f:
        p->slot53(1);
        p->slot33(0);
        p->b0 = 0;
        break;
    case 0x55:
        p->slot53(1);
        p->slot33(0x530);
        p->b0 = 0;
        break;
    case 0x7b:
        p->b5 = 1;
        break;
    case 0x7f:
        p->slot33(0x610);
        p->slot53(1);
        break;
    case 0x27:
    case 0xa2:
    case 0xae:
    case 0xd8:
        p->slot33(0x5f0);
        break;
    case 0x108:
        p->slot33(0x530);
        break;
    case 5:
        p->slot53(1);
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
        p->slot33(0x4a0);
        p->b0 = 0;
        p->b1 = 1;
        if (p->id == 4) {
            p->slot88(1);
        }
        if (p->id == 5) {
            p->slot33(0xf0);
        }
        break;
    case 0xa:
    case 0xb:
    case 0xc:
    case 0xd:
        p->slot53(1);
        p->slot33(0);
        p->b0 = 0;
        p->b1 = 1;
        break;
    case 0x1e:
    case 0x1f:
    case 0x20:
    case 0x101:
        p->slot33(0x920);
        p->b0 = 0;
        p->b2 = 1;
        if (p->id == 0x20) {
            p->slot33(0x400);
        }
        break;
    case 6:
        p->b0 = 0;
        p->b1 = 1;
        break;
    case 0xf6:
        p->slot33(0x940);
        p->b1 = 1;
        p->b6 = 1;
        break;
    case 0xf5:
        p->slot33(0x980);
        p->b1 = 1;
        p->b6 = 1;
        break;
    case 0xf4:
        p->slot33(0x9a0);
        p->b1 = 1;
        p->b6 = 1;
        break;
    case 0x100:
        p->slot33(0x920);
        p->b1 = 1;
        p->b6 = 1;
        break;
    case 0xce:
        p->slot33(0x920);
        p->b1 = 1;
        p->b6 = 1;
        break;
    case 0xeb:
        p->x8a = 1;
        break;
    case 0x6a:
        p->xc4 = 0x20;
        break;
    }
}
