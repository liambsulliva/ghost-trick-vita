class Self {
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
    virtual void slot20(int a, int b, int c, unsigned char d);
};

extern "C" {
void func_02015b24(int *param_1, int param_2, int param_3, int param_4, unsigned char param_5)
{
    Self *self;

    self = (Self *)*param_1;
    self->slot20(param_2, param_3, param_4, param_5);
}
}
