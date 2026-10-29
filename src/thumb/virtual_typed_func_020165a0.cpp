class Object020165a0_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
};

class Object020165a0_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
};

class Object020165a0_2 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
};

extern "C" {
typedef int code(...);
extern int func_020151f0(...);

void func_020165a0(int param_1)

{
    int iVar1;

    iVar1 = func_020151f0();
    if ((iVar1 == 0) && (*(int **)(param_1 + 4) != (int *)0x0)) {
        ((Object020165a0_0 *)(*(int **)(param_1 + 4)))->slot2();
        ((Object020165a0_1 *)(*(int **)(param_1 + 4)))->slot3();
        ((Object020165a0_2 *)(*(int **)(param_1 + 4)))->slot4();
    }
    return;
}
}
