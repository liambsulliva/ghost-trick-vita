class Object020271c8_0 {
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

class Object020271c8_1 {
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

class Object020271c8_2 {
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

class Object020271c8_3 {
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
extern char data_020ce900;
extern char data_020cfe48;
typedef int code(...);
extern int func_02008828(...);
extern int func_020265c4(...);

void func_020271c8(unsigned int param_1,int *param_2,int param_3)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;

    uVar1 = func_020265c4(param_3 + 4);
    uVar2 = func_020265c4(param_3 + 8);
    iVar3 = func_02008828(param_2,((unsigned int)&data_020cfe48));
    if (iVar3 != 0) {
        ((Object020271c8_0 *)param_2)->slot14((int)(uVar1));
        ((Object020271c8_1 *)param_2)->slot15((int)(uVar2));
        return;
    }
    iVar3 = func_02008828(param_2,((unsigned int)&data_020ce900));
    if (iVar3 != 0) {
        ((Object020271c8_2 *)param_2)->slot14((int)(uVar1));
        ((Object020271c8_3 *)param_2)->slot15((int)(uVar2));
    }
    return;
}
}
