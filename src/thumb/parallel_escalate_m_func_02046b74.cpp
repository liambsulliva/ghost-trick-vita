class Obj02046b74 {
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
    char pad[0x40];
    unsigned char flag;
};

extern "C" void func_02046b74(Obj02046b74 *obj)
{
    if (obj->flag != 1) {
        obj->slot10();
        obj->flag = 3;
    }
}
