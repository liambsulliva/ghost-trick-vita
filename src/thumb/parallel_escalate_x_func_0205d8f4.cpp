class Object0205d8f4_1 {
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
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37(int argument0);
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42(int argument0,int argument1,int argument2,int argument3,int argument4,int argument5,int argument6);
};

extern "C" {
void func_0205d8f4(Object0205d8f4_1 *param_1)
{
    int uVar1;

    uVar1 = 0xb;
    if (*(int *)((char *)param_1 + 0x94) != 0) {
        uVar1 = 0xc;
    }
    param_1->slot42(uVar1,0,1,1,1,-1,0);
    param_1->slot37(0x3d);
    param_1->slot33(0x490);
    *(unsigned char *)((char *)param_1 + 0x7e) = 0;
    *(int *)((char *)param_1 + 0x90) = 1;
    *(unsigned int *)((char *)param_1 + 0x80) = *(unsigned char *)((char *)param_1 + 0x7e) << 12;
    *(int *)((char *)param_1 + 0x84) = (*(int *)((char *)param_1 + 0x90) + *(unsigned char *)((char *)param_1 + 0x7e)) << 12;
    *(int *)((char *)param_1 + 0x88) = 0x1b00;
    *(int *)((char *)param_1 + 0x8c) = 0;
    if (*(unsigned char *)((char *)param_1 + 0x98) != 0) {
        *(unsigned char *)((char *)param_1 + 0x7e) = 0x1a;
    }
}
}
