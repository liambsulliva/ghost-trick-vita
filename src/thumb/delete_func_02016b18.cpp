class Object02016b18_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
};

class Object02016b18_1 {
public:
    virtual ~Object02016b18_1();
};

extern "C" {
typedef int code(...);

void func_02016b18(int param_1)

{
    if (*(int **)(param_1 + 4) != (int *)0x0) {
        ((Object02016b18_0 *)(*(int **)(param_1 + 4)))->slot6();
    }
    if (*(int **)(param_1 + 0xc) != (int *)0x0) {
        delete ((Object02016b18_1 *)(*(int **)(param_1 + 0xc)));
        *(unsigned int *)(param_1 + 0xc) = 0;
    }
    *(unsigned int *)(param_1 + 4) = 0;
    return;
}
}
