class Object02015908_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
};

class Object02015908_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual int slot6();
};

extern "C" {
typedef int code(...);

unsigned int func_02015908(unsigned int * param_1)

{


    unsigned int uVar1;

    ((Object02015908_0 *)(*param_1))->slot6();
    uVar1 = ((Object02015908_1 *)(param_1[1]))->slot6();
    return uVar1;
}
}
