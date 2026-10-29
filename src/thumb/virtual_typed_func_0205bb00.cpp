class Object0205bb00_0 {
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

class Object0205bb00_1 {
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
    virtual void slot15(int argument0);
};

extern "C" {
typedef int code(...);
extern int func_0202f64c(...);
extern int func_0205b838(...);
extern int func_0205bb40(...);

void func_0205bb00(int param_1)

{
    int iVar1;

    if (*(int *)(param_1 + 0x34) != 0) {
        if (*(int **)(param_1 + 0x30) != (int *)0x0) {
            ((Object0205bb00_0 *)(*(int **)(param_1 + 0x30)))->slot11();
        }
        func_0202f64c(param_1 + 0x30);
        *(unsigned int *)(param_1 + 0x30) = *(unsigned int *)(param_1 + 0x34);
        *(unsigned int *)(param_1 + 0x34) = 0;
        iVar1 = func_0205b838(param_1);
        ((Object0205bb00_1 *)(*(int **)(param_1 + 0x30)))->slot15((int)(iVar1 << 0xc));
        func_0205bb40(param_1);
    }
    return;
}
}
