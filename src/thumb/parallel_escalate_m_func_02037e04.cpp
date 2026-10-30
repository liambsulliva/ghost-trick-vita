class Obj02037e04 {
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
};

extern "C" {
extern char data_020cf75c;
extern void *func_02009900(void);
extern Obj02037e04 *func_0202fb48(void *system, void *kind, int start);
extern Obj02037e04 *func_02056964(void *kind);
extern void func_0202f7e8(void *system, Obj02037e04 *object, int index, int flag);
extern void func_02056d68(Obj02037e04 *object, int value, unsigned char mode);

void func_02037e04(void *self, int value, int extra)
{
    Obj02037e04 *object;

    object = func_0202fb48(func_02009900(), &data_020cf75c, -1);
    if (object == 0) {
        object = func_02056964(&data_020cf75c);
        func_0202f7e8(func_02009900(), object, 5, 0);
    }
    if (object != 0) {
        object->slot33(extra);
        func_02056d68(object, value, 0);
    }
}
}
