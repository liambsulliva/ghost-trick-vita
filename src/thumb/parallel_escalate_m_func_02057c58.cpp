class Obj02057c58 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int a);
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
    virtual void slot78(int a);
    virtual void slot79();
    virtual void slot80();
    virtual void slot81();
    virtual void slot82();
    virtual void slot83();
    virtual void slot84();
    virtual void slot85();
    virtual void slot86();
    virtual void slot87();
    virtual void slot88();
    virtual unsigned int slot89();
};

extern "C" {
extern void *func_02008444(void);
extern int func_0200f330(void *system);
extern int func_02011320(void *system);
extern void func_020660c4(unsigned char *p);

void func_02057c58(unsigned char *p)
{
    unsigned int count;
    unsigned int state;
    Obj02057c58 *obj;

    state = *(unsigned int *)(p + 0xdc);
    if (state < 2 || state > 4 || *(unsigned int *)(p + 0xd8) <= state) {
        ((Obj02057c58 *)p)->slot8(0);
        return;
    }
    p[0xe6] = 0;
    count = ((Obj02057c58 *)p)->slot89();
    if (func_0200f330(func_02008444()) != 0 || func_02011320(func_02008444()) != 3) {
        ((Obj02057c58 *)p)->slot11();
        obj = *(Obj02057c58 **)(p + 0xe0);
        if (obj != 0) {
            obj->slot11();
            p[0xe6] = 0;
        }
        if (count < 0x32) {
            ((Obj02057c58 *)p)->slot78(0);
        } else {
            ((Obj02057c58 *)p)->slot8(0);
            return;
        }
    } else {
        ((Obj02057c58 *)p)->slot10();
        obj = *(Obj02057c58 **)(p + 0xe0);
        if (obj != 0) {
            obj->slot10();
        }
    }
    func_020660c4(p);
    if (p[0xe6] == 0 && p[0xe7] != 0) {
        (*(Obj02057c58 **)(p + 0xe0))->slot43();
        p[0xe7] = 0;
    }
}
}
