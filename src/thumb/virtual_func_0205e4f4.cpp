class Object0205e4f4_0 {
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
    virtual void slot46(int argument0,int argument1);
};

class Object0205e4f4_1 {
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

class Object0205e4f4_2 {
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

extern "C" {
extern char data_020cfa64;
extern char data_020cfad4;
typedef int code(...);
extern int func_0200847c(...);
extern int func_02009900(...);
extern int func_020153d8(...);
extern int func_0202a0c0(...);
extern int func_0202f7e8(...);
extern int func_02056b18(...);
extern int func_0205bff4(...);
extern int func_0205c008(...);
extern int func_0205e0dc(...);
extern int func_0205e0e8(...);
extern int func_0205e100(...);

void func_0205e4f4(int *param_1)

{
    int iVar1;
    unsigned int uVar2;

    ((Object0205e4f4_0 *)param_1)->slot46((int)(0x100000),(int)(0xc0000));
    func_02056b18(param_1,0x1f);
    ((Object0205e4f4_1 *)param_1)->slot35((int)(0x1f));
    ((Object0205e4f4_2 *)param_1)->slot33((int)(0x470));
    *(unsigned char *)(param_1 + 0x34) = 0;
    iVar1 = func_0205bff4(((unsigned int)&data_020cfad4));
    param_1[0x32] = iVar1;
    if (iVar1 != 0) {
        func_0205e0dc(iVar1,0,0);
        func_0205e0e8(param_1[0x32],0x1f);
        func_020153d8(param_1[0x32],param_1);
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,param_1[0x32],7,0);
    }
    iVar1 = func_0205c008(((unsigned int)&data_020cfa64));
    param_1[0x33] = iVar1;
    if (iVar1 != 0) {
        func_0205e100(iVar1,((unsigned int)0x00007fffu),0);
        func_020153d8(param_1[0x33],param_1);
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,param_1[0x33],7,0);
    }
    param_1[0x35] = ((int)0x000004cdu);
    param_1[0x36] = 0x52;
    param_1[0x37] = 0;
    param_1[0x38] = 0;
    param_1[0x39] = 0x2000;
    param_1[0x3a] = ((int)0xfffffe66u);
    uVar2 = func_0200847c();
    func_0202a0c0(uVar2,0xe);
    return;
}
}
