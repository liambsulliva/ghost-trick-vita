class Object02056b7c {
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
    virtual void slot37(int param_1);
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46(int param_1, int param_2);
};

extern "C" {
extern unsigned int data_020ade7c;
unsigned int *func_02056ac8(unsigned int *param_1);
void func_02056b18(int param_1, unsigned short param_2);

unsigned int *func_02056b7c(unsigned int *param_1)
{
    unsigned int v;

    func_02056ac8(param_1);
    *param_1 = (unsigned int)&data_020ade7c;
    func_02056b18((int)param_1, 0);
    v = 0x10;
    *(unsigned char *)((int)param_1 + 0x44) = v;
    ((Object02056b7c *)param_1)->slot37(0x3e);
    ((Object02056b7c *)param_1)->slot46(v << 16, 3 << 18);
    return param_1;
}
}
