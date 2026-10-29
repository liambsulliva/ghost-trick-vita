class Object02066074_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual int slot5();
};

class Object02066074_1 {
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
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53();
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60();
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64(int argument0,int argument1);
};

extern "C" {
typedef int code(...);
extern int func_0200b208(...);
extern int func_02023f24(...);
extern int func_0204235c(...);
extern int func_020575b8(...);

void func_02066074(int *param_1,unsigned int param_2)

{
    unsigned int uVar1;
    int iVar2;
    int *piVar3;

    uVar1 = func_0200b208();
    func_02023f24(uVar1,param_1[0x34]);
    *(short *)(param_1 + 0x22) = (short)param_2;
    iVar2 = func_0204235c(param_2);
    param_1[0x34] = iVar2;
    uVar1 = func_0200b208();
    piVar3 = (int *)func_020575b8(uVar1,param_1[0x34]);
    if (piVar3 != (int *)0x0) {
        uVar1 = ((Object02066074_0 *)piVar3)->slot5();
        ((Object02066074_1 *)param_1)->slot64((int)(uVar1),(int)(piVar3));
    }
    return;
}
}
