class Object0204c5c4_0 {
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
    virtual void slot14(int argument0);
};

class Object0204c5c4_1 {
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
extern int func_0204c640(...);

void func_0204c5c4(int param_1)

{
    unsigned short uVar1;

    if (*(short *)(param_1 + 0xb6) < 0) {
        if (*(unsigned short *)(param_1 + 0x88) == ((unsigned int)0x00008013u)) {
            uVar1 = 0x18;
        }
        else {
            uVar1 = 0x1f;
        }
        *(unsigned short *)(param_1 + 0xb6) = uVar1;
    }
    if (*(unsigned short *)(param_1 + 0x88) == ((unsigned int)0x00008014u)) {
        func_0204c640(*(int *)(param_1 + 0xb8),((unsigned int)0xfff80000u),((unsigned int)0xfffa0000u),0,0x60000);
        ((Object0204c5c4_0 *)(*(int **)(param_1 + 0xb8)))->slot14((int)(*(unsigned int *)(param_1 + 0x28)));
    }
    ((Object0204c5c4_1 *)(*(int **)(param_1 + 0xb8)))->slot35((int)((int)(signed char)*(short *)(param_1 + 0xb6)));
    return;
}
}
