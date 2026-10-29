class Object0201c248 {
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
    virtual void slot14(int argument0);
    virtual void slot15(int argument0);
    int pad[9];
    int field10;
    int field11;
};

extern "C" {
void func_0201c248(Object0201c248 *object,int a,int b)
{
    object->slot14(object->field10 + a);
    object->slot15(object->field11 - b);
}
}
