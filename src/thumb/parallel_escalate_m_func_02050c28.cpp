class Base02050c28 {
public:
    virtual void base0();
    char pad[0x24];
};

class Sub02050c28 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual int slot5(int value, int other);
};

class Obj02050c28 : public Base02050c28, public Sub02050c28 {
};

extern "C" int func_02050c28(Obj02050c28 *obj, int value, int other)
{
    return obj->slot5(value, other);
}
