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
    virtual void slot44(int);
};
extern "C" {
extern void func_ov006_020d4d28(void *);
extern void *func_0200883c();
extern int func_0201a4e8(void *);
extern int func_0201e888(int, int, int, int, int);
extern int func_0201003c();
extern void func_02015c9c(int, int, int);
extern void func_020427b0(int, int, int, int, int, int, int, int, int, int, int);
void func_ov006_020d4cac(Object *p, int a, int b)
{
    func_ov006_020d4d28(p);
    int value = func_0201a4e8(func_0200883c());
    *(int *)((unsigned char *)p + 0xc8) = func_0201e888(value, 0, b, 0, 0);
    func_02015c9c(func_0201003c(), a, *(int *)((unsigned char *)p + 0xc8));
    func_020427b0(*(int *)((unsigned char *)p + 0xcc), *(int *)((unsigned char *)p + 0xc8), 0, 0, 1, 0, 0xffff, 1, -1, -1, 0x100);
    p->slot44(*(int *)((unsigned char *)p + 0xcc));
}
}
