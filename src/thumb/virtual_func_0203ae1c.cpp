class Object0203ae1c_0 {
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
    virtual void slot30(int argument0);
};

extern "C" {
typedef int code(...);
extern int func_0203ad9c(...);
extern int func_0203add8(...);

void func_0203ae1c(int *param_1)

{
    unsigned char uVar1;
    unsigned short uVar2;

    uVar1 = func_0203add8();
    *(unsigned char *)((int)param_1 + 0xdd) = uVar1;
    uVar2 = func_0203ad9c(param_1,*(unsigned char *)((int)param_1 + 0xdd));
    *(unsigned short *)((int)param_1 + 0xda) = uVar2;
    ((Object0203ae1c_0 *)param_1)->slot30((int)(0));
    (**(code **)(*(int *)param_1[0x3e] + 0x78))
                        ((int *)param_1[0x3e],*(unsigned short *)((int)param_1 + 0xda));
    return;
}
}
