class Object0204d3d4_0 {
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
    virtual void slot47(int argument0,int argument1,int argument2);
};

extern "C" {
typedef int code(...);
extern int func_020082d8(...);
extern int func_020471ec(...);

void func_0204d3d4(int param_1,int unused_2,unsigned int param_3,unsigned int param_4)

{




    int iVar1;
    unsigned int auStack_1c [3];
    unsigned int auStack_28 [3];

    func_020471ec(auStack_1c,param_1,param_3,param_4);
    iVar1 = 0x10;
    if (0x10 < *((int *)0x020ceaa4u)) {
        iVar1 = *((int *)0x020ceaa4u);
    }
    func_020082d8(auStack_28,auStack_1c);
    ((Object0204d3d4_0 *)(*(int **)(param_1 + 0xb8)))->slot47((int)(auStack_28),(int)(0x800),(int)(iVar1 << 0xc));
    return;
}
}
