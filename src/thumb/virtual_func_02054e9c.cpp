class Object02054e9c_0 {
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
    virtual void slot64();
    virtual void slot65(int argument0);
};

extern "C" {
extern char data_020cfe10;
typedef int code(...);
extern int func_02008444(...);
extern int func_02009900(...);
extern int func_0201003c(...);
extern int func_020153d8(...);
extern int func_02015c6c(...);
extern int func_0202f7e8(...);
extern int func_02054ef8(...);
extern int func_02065aa4(...);

void func_02054e9c(int param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int *piVar3;

    func_02008444();
    uVar1 = func_02054ef8();
    uVar2 = func_0201003c();
    func_02015c6c(uVar2,uVar1,0);
    piVar3 = (int *)func_02065aa4(((unsigned int)&data_020cfe10));
    *(int **)(param_1 + 0x30) = piVar3;
    ((Object02054e9c_0 *)piVar3)->slot65((int)(0x36));
    (**(code **)(**(int **)(param_1 + 0x30) + 0x84))(*(int **)(param_1 + 0x30),((unsigned int)0x0000044fu));
    func_020153d8(*(unsigned int *)(param_1 + 0x30),param_1);
    uVar1 = func_02009900();
    func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 0x30),1,0);
    return;
}
}
