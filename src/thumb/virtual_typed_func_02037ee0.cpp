class Object02037ee0_0 {
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
    virtual void slot12();
};

class Object02037ee0_1 {
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
    virtual void slot12();
};

class Object02037ee0_2 {
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
    virtual void slot12();
};

extern "C" {
typedef int code(...);
extern int func_02037f10(...);

void func_02037ee0(int param_1)

{
    func_02037f10(*(unsigned int *)(param_1 + 0x40));
    func_02037f10(*(unsigned int *)(param_1 + 0x44));
    ((Object02037ee0_0 *)(*(int **)(param_1 + 0x48)))->slot12();
    ((Object02037ee0_1 *)(*(int **)(param_1 + 0x4c)))->slot12();
    if (*(int **)(param_1 + 0x3c) != (int *)0x0) {
        ((Object02037ee0_2 *)(*(int **)(param_1 + 0x3c)))->slot12();
    }
    return;
}
}
