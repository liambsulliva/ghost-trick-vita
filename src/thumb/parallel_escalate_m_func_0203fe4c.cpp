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
};
extern "C" {
extern void func_0202ff40(Actor *actor);
extern int func_02010b54(Actor *actor);
extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *object);
void func_0203fe4c(Actor *actor)
{
    unsigned char *p = (unsigned char *)actor;
    func_0202ff40(actor);
    if (p[0xcc] != 0) {
        p[0xcc]--;
        return;
    }
    if (func_02010b54(actor) == 0) {
        actor->slot10();
    } else if ((func_01ff80b8(func_02008eb0()) & 1) == 0) {
        actor->slot11();
        p[0xcc] = 10;
        return;
    }
    p[0xcc] = 30;
}
}
