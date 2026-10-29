class Object02039264_0 {
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
};

class Object02039264_1 {
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
    virtual int slot10();
};

extern "C" {
typedef int code(...);

unsigned int func_02039264(int * param_1)

{


    unsigned int uVar1;

    ((Object02039264_0 *)(param_1))->slot10();
    uVar1 = ((Object02039264_1 *)(param_1[0x33]))->slot10();
    return uVar1;
}
}
