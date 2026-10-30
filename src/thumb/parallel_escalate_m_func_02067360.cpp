class Object {
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

extern "C" int func_02008444(...);
extern "C" int func_0200f330(...);
extern "C" unsigned int func_02011320(...);
extern "C" void func_020660c4(...);

extern "C" void func_02067360(int *param_1)
{
    if (func_0200f330(func_02008444()) != 0 || func_02011320(func_02008444()) != 3) {
        ((Object *)param_1)->slot11();
    }
    else {
        ((Object *)param_1)->slot10();
        func_020660c4(param_1);
    }
}
