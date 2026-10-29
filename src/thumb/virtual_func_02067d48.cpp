class Object02067d48_0 {
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

extern "C" {
typedef int code(...);
extern int func_02010b7c(...);
extern int func_0202f174(...);

void func_02067d48(int *param_1)

{
    unsigned int uVar1;

    uVar1 = func_02010b7c();
    func_0202f174(uVar1,param_1 + 0x2f);
    ((Object02067d48_0 *)param_1)->slot10();
    return;
}
}
