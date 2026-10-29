class Object02016634_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
};

class Object02016634_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
};

extern "C" {
typedef int code(...);

void func_02016634(int param_1)

{
    if (*(int **)(param_1 + 0x10) != (int *)0x0) {
        ((Object02016634_0 *)(*(int **)(param_1 + 0x10)))->slot7();
    }
    if (*(int **)(param_1 + 0xc) != (int *)0x0) {
        ((Object02016634_1 *)(*(int **)(param_1 + 0xc)))->slot7();
    }
    return;
}
}
