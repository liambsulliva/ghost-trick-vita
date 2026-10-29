class Object02057994_0 {
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
    virtual void slot42(int argument0,int argument1,int argument2,int argument3,int argument4,int argument5,int argument6);
};

class Object02057994_1 {
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
    virtual void slot33(int argument0);
};

class Object02057994_2 {
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
    virtual void slot13(int argument0,int argument1,int argument2);
};

extern "C" {
extern char data_020cf724;
typedef int code(...);
extern int func_02009900(...);
extern int func_0202f7e8(...);
extern int func_0205693c(...);

int * func_02057994(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4,
                                      unsigned int param_5)

{
    int *piVar1;
    unsigned int uVar2;

    piVar1 = (int *)func_0205693c(((unsigned int)&data_020cf724));
    if (piVar1 != (int *)0x0) {
        ((Object02057994_0 *)piVar1)->slot42((int)(param_1),(int)(0),(int)(1),(int)(0),(int)(1),(int)(0xffffffff),(int)(0));
        ((Object02057994_1 *)piVar1)->slot33((int)(param_2));
        ((Object02057994_2 *)piVar1)->slot13((int)(param_3),(int)(param_4),(int)(0));
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,piVar1,param_5,0);
    }
    return piVar1;
}
}
