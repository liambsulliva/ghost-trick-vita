class Obj02034a6c {
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
    virtual void slot41(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
};

extern "C" {
extern char data_020cd988;
extern Obj02034a6c *func_020343ec(void *name);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Obj02034a6c *object, int index, int flag);

void func_02034a6c(void *self, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)
{
    Obj02034a6c *object;

    if (a != 0) {
        object = func_020343ec(&data_020cd988);
        if (object != 0) {
            object->slot41(a, b, c, d, e, f, g, h, i, j);
            func_0202f7e8(func_02009900(), object, 7, 0);
        }
    }
}
}
