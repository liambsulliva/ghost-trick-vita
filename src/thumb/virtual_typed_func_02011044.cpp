class Object02011044_0 {
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
typedef int code(...);

void func_02011044(int param_1)

{
    if (*(int **)(param_1 + ((int)0x00000fb8u)) != (int *)0x0) {
        ((Object02011044_0 *)(*(int **)(param_1 + ((int)0x00000fb8u))))->slot11();
    }
    return;
}
}
