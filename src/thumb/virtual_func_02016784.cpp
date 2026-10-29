class Object02016784_0 {
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
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
};

extern "C" {
typedef int code(...);
extern int func_020169a4(...);

void func_02016784(int param_1)

{
    int *piVar1;

    piVar1 = (int *)func_020169a4(param_1,0);
    *(int **)(param_1 + 4) = piVar1;
    if (piVar1 != (int *)0x0) {
        ((Object02016784_0 *)piVar1)->slot24();
    }
    return;
}
}
