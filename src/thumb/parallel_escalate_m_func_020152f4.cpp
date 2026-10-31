class Actor {
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
    virtual void slot13(int x, int y, int z);
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
    virtual void slot33(int value);
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
    virtual void slot65(int value);
};

extern "C" {
extern char data_020ce91c;
extern char data_020cfd44;
extern char data_020cfe2c;
extern void *func_02008444(void);
extern void *func_02009900(void);
extern void *func_0201003c(void);
extern void func_02010384(void *object, int value);
extern void func_02014e24(unsigned char *param_1);
extern void func_020153d8(Actor *object, unsigned char *param_1);
extern void func_02015418(void *object);
extern void func_0202f7e8(void *object, void *value, int param_3, int param_4);
extern int func_0202fa8c(void *object, void *data);
extern void *func_02046f68(void *data);
extern Actor *func_02065ab8(void *data);

void func_020152f4(unsigned char *param_1)
{
    Actor *actor;

    switch (param_1[0x10]) {
    case 0:
        *(void **)(param_1 + 0x5c) = func_02046f68(&data_020ce91c);
        func_0202f7e8(func_02009900(), *(void **)(param_1 + 0x5c), 1, 0);
        param_1[0x10] = param_1[0x10] + 1;
        return;
    case 1:
        if (param_1[0x62] != 0 ||
            (param_1[0x61] == 0 && func_0202fa8c(func_02009900(), &data_020cfd44) != 0)) {
            func_02010384(func_02008444(), 1);
            actor = func_02065ab8(&data_020cfe2c);
            *(Actor **)(param_1 + 0x58) = actor;
            if (actor != 0) {
                func_020153d8(actor, param_1);
                (*(Actor **)(param_1 + 0x58))->slot65(0x123);
                (*(Actor **)(param_1 + 0x58))->slot33(0x470);
                (*(Actor **)(param_1 + 0x58))->slot13(0x40000, 0x18000, 0);
                func_0202f7e8(func_02009900(), *(Actor **)(param_1 + 0x58), 7, 0);
            }
            param_1[0x11] = 0xb4;
            param_1[0x10] = param_1[0x10] + 1;
        }
        return;
    case 2:
        param_1[0x11] = param_1[0x11] - 1;
        if (param_1[0x11] == 0) {
            func_02015418(func_0201003c());
            func_02014e24(param_1);
        }
        return;
    }
}
}
