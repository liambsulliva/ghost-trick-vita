class Object020165e4_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
};

class Object020165e4_1 {
public:
    virtual ~Object020165e4_1();
};

class Object020165e4_2 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
};

extern "C" {
typedef int code(...);

void func_020165e4(int param_1)

{
    if (*(int **)(param_1 + 0x10) != (int *)0x0) {
        ((Object020165e4_0 *)(*(int **)(param_1 + 0x10)))->slot6();
    }
    if (*(int **)(param_1 + 0x10) != (int *)0x0) {
        delete ((Object020165e4_1 *)(*(int **)(param_1 + 0x10)));
        *(unsigned int *)(param_1 + 0x10) = 0;
    }
    if (*(int **)(param_1 + 0xc) != (int *)0x0) {
        ((Object020165e4_2 *)(*(int **)(param_1 + 0xc)))->slot6();
    }
    *(unsigned int *)(param_1 + 4) = 0;
    return;
}
}
