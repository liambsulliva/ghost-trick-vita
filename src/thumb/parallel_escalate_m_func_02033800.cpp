class Item02033800 {
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
};

class Self02033800 {
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
    virtual int slot11();
    char pad04[0x1c];
    void *field20;
    char sub24[4];
};

extern "C" {
extern char data_020cf87c;
extern void func_020326a4(Self02033800 *self);
extern void *func_02008444(void);
extern void func_0201192c(void *object, int value);
extern void func_02010ef4(void *object, int value, int extra);
extern void func_0202fbcc(void *iterator, int kind);
extern void *func_0200eab4(void *iterator);
extern int func_02008828(void *object, void *type);
extern Item02033800 *func_0200eab8(void *iterator);
extern void func_0200eabc(void *iterator, int value);
extern int func_0200eac4(void *iterator);
extern void func_0203388c(void *object, int value);
extern void func_02032900(void *object, int value);
extern void func_0202f64c(void *object);

void func_02033800(Self02033800 *self)
{
    unsigned char iterator[16];

    func_020326a4(self);
    func_0201192c(func_02008444(), 5);
    func_02010ef4(func_02008444(), 0xff, -1);
    func_0202fbcc(iterator, 6);
    while (func_0200eac4(iterator) != 0) {
        if (func_02008828(func_0200eab4(iterator), &data_020cf87c) != 0) {
            func_0200eab8(iterator)->slot14();
        }
        func_0200eabc(iterator, 0);
    }
    func_0203388c(self->field20, 0);
    if (self->slot11() != 0) {
        func_02032900(self->field20, 1);
    }
    func_0202f64c(self->sub24);
}
}
