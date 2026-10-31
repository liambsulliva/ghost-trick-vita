class Child
{
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
    virtual void slot35(int value);
};

class Self
{
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
    virtual void slot66();
    virtual void slot67();
    virtual void slot68();
    virtual void slot69();
    virtual void slot70();
    virtual void slot71();
    virtual unsigned short slot72(char *name);
};

extern "C" {
extern char data_0209d8e4;
extern char data_020cf724;
Child *func_0205693c(char *name);
void func_020153d8(int object, int value);
unsigned char *func_02009900(void);
void func_0202f7e8(int param_1, int param_2, unsigned int param_3, int param_4);

void func_02057b94(unsigned char *p)
{
    Child *obj;
    int base;
    int low;

    base = 0x11e;
    ((Self *)p)->slot65(base);
    ((Self *)p)->slot33(0x470);
    *(unsigned short *)(p + 0xe4) = ((Self *)p)->slot72(&data_0209d8e4);
    obj = func_0205693c(&data_020cf724);
    *(Child **)(p + 0xe0) = obj;
    func_020153d8((int)obj, (int)p);
    (*(Child **)(p + 0xe0))->slot35(0);
    func_0202f7e8((int)func_02009900(), *(int *)(p + 0xe0), 7, 0);
    low = -0x4000;
    *(int *)(p + 0xf4) = low;
    *(int *)(p + 0xe8) = low;
    *(int *)(p + base - 0x12) = 0x4000;
    *(int *)(p + 0x100) = 0x4000;
    *(int *)(p + base - 0xe) = low;
    *(int *)(p + 0xec) = low;
    *(int *)(p + 0x104) = 0x4000;
    *(int *)(p + 0xf8) = 0x4000;
    *(int *)(p + base - 0xa) = 0x1000;
    *(int *)(p + base - 0x16) = 0x1000;
    *(int *)(p + 0xfc) = 0x1000;
    *(int *)(p + 0xf0) = 0x1000;
}
}
