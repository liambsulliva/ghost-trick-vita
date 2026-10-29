class Object02037e54_0 {
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
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
};

class Object02037e54_1 {
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
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
};

class Object02037e54_2 {
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
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
};

extern "C" {
typedef int code(...);
extern int func_02008444(...);
extern int func_020322d4(...);
extern int func_02037e90(...);

void func_02037e54(int param_1)

{
    int iVar1;

    func_02037e90(*(unsigned int *)(param_1 + 0x40));
    func_02037e90(*(unsigned int *)(param_1 + 0x44));
    ((Object02037e54_0 *)(*(int **)(param_1 + 0x48)))->slot15();
    ((Object02037e54_1 *)(*(int **)(param_1 + 0x4c)))->slot15();
    func_02008444();
    iVar1 = func_020322d4();
    if ((iVar1 == 0) && (*(int **)(param_1 + 0x3c) != (int *)0x0)) {
        ((Object02037e54_2 *)(*(int **)(param_1 + 0x3c)))->slot15();
    }
    return;
}
}
