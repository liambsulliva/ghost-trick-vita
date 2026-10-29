class Object020467dc_0 {
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
    virtual int slot11();
};

extern "C" {
typedef int code(...);
extern int func_0200840c(...);

unsigned int func_020467dc(int param_1)

{


    unsigned int uVar1;
    unsigned int uVar2;

    func_0200840c();
    uVar2 = 0;
    do {
        uVar1 = ((Object020467dc_0 *)(*(int **)(param_1 + uVar2 * 4 + 0x28)))->slot11();
        uVar2 = uVar2 + 1;
    } while (uVar2 < 2);
    return uVar1;
}
}
