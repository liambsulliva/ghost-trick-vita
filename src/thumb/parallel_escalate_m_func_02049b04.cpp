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
    virtual void slot14(int value);
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
    virtual void slot46(int a, int b);
};

extern "C" {
extern void func_0205765c(Object *object, int a, int b, int c, int d);
}

extern "C" void func_02049b04(char *self)
{
    *(int *)(self + 0xc8) = 0;
    if (*(unsigned char *)(self + 0xe9) != 0) {
        *(unsigned char *)(self + 0xea) = 1;
        (*(Object **)(self + 0xb8))->slot11();
        (*(Object **)(self + 0xb8))->slot43();
    }
    func_0205765c(*(Object **)(self + 0xb8), *(int *)(self + 0xe4), 0, *(int *)(self + 0xe4) + 0x80000, 0xc0000);
    (*(Object **)(self + 0xb8))->slot46(0x80000, 0xc0000);
    if (*(unsigned char *)(self + 0xb4) != 0) {
        *(int *)(self + 0xf0) = -0x40000;
        (*(Object **)(self + 0xb8))->slot14(*(int *)(self + 0xf0));
        return;
    }
    *(int *)(self + 0xf0) = 0x40000;
    (*(Object **)(self + 0xb8))->slot14(*(int *)(self + 0xf0));
}
