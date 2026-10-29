class Object020158ec_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
};

class Object020158ec_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2();
};

extern "C" {
typedef int code(...);

unsigned int func_020158ec(unsigned int * param_1)

{


    unsigned int uVar1;

    ((Object020158ec_0 *)(*param_1))->slot2();
    uVar1 = ((Object020158ec_1 *)(param_1[1]))->slot2();
    return uVar1;
}
}
