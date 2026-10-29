class Object020167a0_0 {
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
    virtual void slot20(int argument0,int argument1,int argument2,int argument3);
};

extern "C" {
typedef int code(...);
extern int func_020169a4(...);

void func_020167a0(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4,
                                    unsigned char param_5)

{
    int *piVar1;

    piVar1 = (int *)func_020169a4(param_1,5);
    *(int **)(param_1 + 4) = piVar1;
    if (piVar1 != (int *)0x0) {
        ((Object020167a0_0 *)(piVar1))->slot20((int)(param_2),(int)(param_3),(int)(param_4),(int)(param_5));
    }
    return;
}
}
