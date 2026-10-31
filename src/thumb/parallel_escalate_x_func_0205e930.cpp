class Object0205e930_0 {
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
    virtual void slot35(int argument0);
    virtual void slot36();
    virtual void slot37(int argument0);
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42(int argument0, int argument1, int argument2, int argument3, int argument4, int argument5, int argument6);
};

extern "C" {
extern void func_0205eb18(unsigned char *argument0);
extern void func_020576d0(unsigned char *argument0, int argument1);

void func_0205e930(unsigned char *param_1, int param_2, unsigned int param_3, int param_4, unsigned char param_5, unsigned short param_6, unsigned short param_7, int param_8)
{
    unsigned int i;
    unsigned char *q;
    unsigned int step;
    int k;

    func_0205eb18(param_1);
    ((Object0205e930_0 *)(param_1 + 4))->slot42(param_7, 0, 1, 0, 1, -1, 0);
    func_020576d0(param_1 + 4, param_4);
    k = 0x712;
    ((Object0205e930_0 *)(param_1 + 4))->slot35((int)*(signed char *)(param_1 + k));
    ((Object0205e930_0 *)(param_1 + 4))->slot33(param_6);
    ((Object0205e930_0 *)(param_1 + 4))->slot37(0x3a);
    *(unsigned char *)(param_1 + k - 2) = param_5;
    *(int *)(param_1 + k + 0x12) = param_8;
    *(unsigned int *)(param_1 + k - 6) = param_3;
    if (param_3 > 0x40) {
        *(unsigned int *)(param_1 + k - 6) = 0x40;
    }
    step = 1;
    if (param_2 != 0) {
        step = *(unsigned int *)(param_1 + 0x70c) / (unsigned int)(param_2 - 1) + 1;
    }
    for (i = 0; i < *(unsigned int *)(param_1 + 0x70c); i++) {
        q = param_1 + i;
        q[0x5cc] = i / step;
        if (q[0x5cc] > param_2 - 1) {
            q[0x5cc] = param_2 - 1;
        }
    }
}
}
