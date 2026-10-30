class ObjA020272d0 {
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
    virtual void slot57(int a, unsigned int b, unsigned int c);
};

extern "C" {
extern char data_020cfe48[];
extern char data_020ce900[];
extern unsigned int func_020265c4(unsigned char *bytes);
extern int func_02008828(void *object, void *type);

void func_020272d0(void *script, ObjA020272d0 *object, unsigned char *args)
{
    short a;
    unsigned int b;
    unsigned int c;

    a = *(short *)(args + 4);
    b = func_020265c4(args + 6);
    c = func_020265c4(args + 10);
    if (func_02008828(object, data_020cfe48) != 0) {
        object->slot57(a, b, c);
        return;
    }
    if (func_02008828(object, data_020ce900) != 0) {
        object->slot57(a, b, c);
    }
}
}
