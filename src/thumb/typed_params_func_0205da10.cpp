class Object0205da10_0 {
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
};

extern "C" {
typedef int code(...);
extern int func_02029708(...);
extern unsigned char * func_0202970c(...);
extern int func_0202c358(...);
extern int func_020503f0(...);
extern int func_0205da7c(...);
extern int func_0205da88(...);

void func_0205da10(int *param_1)

{
    int iVar1;
    unsigned char *puVar2;

    iVar1 = func_0202c358((int)param_1);
    if ((iVar1 != 0) && (*(unsigned char *)((int)param_1 + 0x7e) < 0x1a)) {
        func_0205da7c();
        puVar2 = func_0202970c();
        iVar1 = func_02029708(puVar2);
        func_0205da88(0,0,-(iVar1 + -0x490));
        ((Object0205da10_0 *)(param_1))->slot41();
        func_020503f0(param_1[0x23],param_1[0x22],param_1[0x20],param_1[0x21],
                                    *(unsigned short *)(param_1 + 0x1f),*(unsigned short *)(param_1 + 0x1f),'\x01');
    }
    return;
}
}
