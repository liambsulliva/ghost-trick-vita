class Object020443dc_0 {
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
    virtual int slot47(int argument0,int argument1);
};

extern "C" {
typedef int code(...);
extern int func_02010b60(...);
extern int func_02010b64(...);
extern int func_02010b7c(...);
extern int func_02015154(...);

unsigned int func_020443dc(int *param_1)

{
    int iVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    func_02010b7c();
    iVar1 = func_02015154();
    if (iVar1 != 0) {
        func_02010b7c();
        uVar2 = func_02010b60();
        func_02010b7c();
        uVar3 = func_02010b64();
        iVar1 = ((Object020443dc_0 *)param_1)->slot47((int)(uVar2),(int)(uVar3));
        if (iVar1 != 0) {
            return 1;
        }
    }
    return 0;
}
}
