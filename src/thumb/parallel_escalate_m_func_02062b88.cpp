class Object {
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
    virtual void slot33(int);
};

struct Table5 {
    unsigned short value[5];
};

extern "C" {
extern struct Table5 data_0209dcf0;
extern struct Table5 data_0209dce4;
extern void func_0204c2fc(...);
}

extern "C" void func_02062b88(Object *param_1,int param_2)
{
    struct Table5 first;
    struct Table5 second;

    if (param_2 < 0) {
        param_2 = 0;
    }
    if (param_2 > 4) {
        param_2 = 4;
    }
    first = data_0209dcf0;
    second = data_0209dce4;
    func_0204c2fc(param_1,second.value[param_2]);
    param_1->slot33(first.value[param_2]);
}
