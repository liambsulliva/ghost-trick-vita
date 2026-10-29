class Object02018464_0 {
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
    virtual int slot25();
};

extern "C" {
typedef int code(...);
extern int func_02008444(...);
extern int func_02018448(...);

unsigned int func_02018464(int *param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((Object02018464_0 *)param_1)->slot25();
    if (iVar1 == 0) {
        uVar2 = func_02008444();
        func_02018448(uVar2,1);
    }
    return 0;
}
}
