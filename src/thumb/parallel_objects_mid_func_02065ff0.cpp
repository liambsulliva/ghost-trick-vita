class Object02065ff0_0 {
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
    virtual void slot65();
    virtual void slot66();
};

extern "C" {
typedef int code(...);
extern char data_020ca29c;
extern int func_02010b54(...);
extern int func_0202b244(...);
extern int func_0202cb48(...);
extern int func_0202cb58(...);
extern int func_0206606c(...);

void func_02065ff0(int *param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;
    int scale;
    int *child;
    unsigned int saved2;

    saved2 = param_2;
    scale = 0x42;
    scale = scale << 2;
    ((Object02065ff0_0 *)param_1)->slot66();
    iVar1 = func_0202b244((int)&data_020ca29c);
    param_1[0x32] = iVar1;
    scale = scale + 0x80;
    *(int *)((char *)iVar1 + scale) = (int)param_1;
    *(unsigned char *)(param_1 + 0x33) = 0;
    *(int **)((char *)param_1[0x32] + 0xb4) = param_1 + 0x33;
    (**(code **)(*(int *)param_1[0x32] + 0xa0))((int *)param_1[0x32],saved2,param_3);
    func_0206606c(param_1[0x32],*(unsigned short *)(param_1 + 0x22));
    if (func_02010b54((int)param_1) != 0) {
        func_0202cb48(param_1[0x32]);
        return;
    }
    func_0202cb58(param_1[0x32]);
    return;
}
}
