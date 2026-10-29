class Object02056394_0 {
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
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35(int argument0);
};

class Object02056394_1 {
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
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35(int argument0);
};

extern "C" {
typedef int code(...);
extern unsigned long long func_0207b694(...);

void func_02056394(int param_1,int param_2)

{
    int *piVar1;
    int iVar2;
    unsigned long long uVar3;

    uVar3 = func_0207b694(param_2 * 0x1f,100);
    iVar2 = (int)(signed char)((int)uVar3 >> 0xc);
    piVar1 = *(int **)(param_1 + 0x34);
    if (piVar1 != (int *)0x0) {
        ((Object02056394_0 *)(piVar1))->slot35((int)(iVar2));
    }
    piVar1 = *(int **)(param_1 + 0x38);
    if (piVar1 != (int *)0x0) {
        ((Object02056394_1 *)(piVar1))->slot35((int)(iVar2));
    }
    return;
}
}
