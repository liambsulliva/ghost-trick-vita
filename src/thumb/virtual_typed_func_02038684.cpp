class Object02038684_0 {
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

class Object02038684_1 {
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

void func_02038684(int param_1)

{
    if (*(int **)(param_1 + 4) != (int *)0x0) {
        ((Object02038684_0 *)(*(int **)(param_1 + 4)))->slot10();
    }
    if (*(int **)(param_1 + 8) != (int *)0x0) {
        ((Object02038684_1 *)(*(int **)(param_1 + 8)))->slot10();
    }
    return;
}
}
