class Object02044d7c_0 {
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
    virtual void slot44(int argument0);
};

extern "C" {
typedef int code(...);
extern int func_020428f0(...);
extern int func_02042a68(...);

void func_02044d7c(int *param_1,unsigned char param_2,int param_3)

{
    if (param_1[0x37] != 0) {
        func_02042a68();
        func_020428f0(param_1[0x37],param_1[0x40] + param_3 * 2,param_1[0x41],2,0,((unsigned int)0x0000ffffu),1,
                                  0xffffffff,0x100);
        ((Object02044d7c_0 *)param_1)->slot44((int)(param_1[0x37]));
        *(unsigned char *)((int)param_1 + 0xee) = param_2;
        *(unsigned char *)((int)param_1 + 0xef) = 0;
    }
    return;
}
}
