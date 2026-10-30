class Object02047398_0 {
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
    virtual void slot35(int argument0);
};

struct Mover {
    char pad[0xb8];
    int position;
    int speed;
    Object02047398_0 *target;
};

extern "C" {
extern int func_0202ff58(...);

void func_02047398(Mover *mover)
{
    if (func_0202ff58(mover) != 0 && mover->target != 0) {
        mover->position += mover->speed;
        if (mover->position > 0x1f000) {
            mover->position = 0x1f000;
        }
        mover->target->slot35((signed char)(mover->position >> 12));
    }
}
}
