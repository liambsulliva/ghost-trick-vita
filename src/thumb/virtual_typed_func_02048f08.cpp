class Object02048f08_0 {
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
};

extern "C" {
typedef int code(...);
extern int func_02008444(...);
extern int func_02010b54(...);
extern int func_0202c394(...);
extern int func_0203435c(...);

void func_02048f08(int param_1)

{
    int iVar1;

    func_02008444();
    iVar1 = func_0202c394();
    if (iVar1 == 0) {
        func_02008444();
        iVar1 = func_0203435c();
        if ((iVar1 == 0) && (iVar1 = func_02010b54(param_1), iVar1 != 0)) {
            ((Object02048f08_0 *)(*(int **)(param_1 + 0xb8)))->slot32();
        }
    }
    return;
}
}
