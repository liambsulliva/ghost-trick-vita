class Object {
public:
    virtual void slot0(int value);
    virtual void slot1(int value);
    virtual void slot2(int value);
    virtual void slot3(int value);
    virtual void slot4(int value);
    virtual void slot5(int value);
    virtual void slot6(int value);
    virtual void slot7(int value);
    virtual void slot8(int value);
    virtual void slot9(int value);
    virtual void slot10(int value);
    virtual void slot11(int value);
    virtual void slot12(int value);
    virtual void slot13(int value);
    virtual void slot14(int value);
    virtual void slot15(int value);
    virtual void slot16(int value);
    virtual void slot17(int value);
    virtual void slot18(int value);
    virtual void slot19(int value);
    virtual void slot20(int value);
    virtual void slot21(int value);
    virtual void slot22(int value);
    virtual void slot23(int value);
    virtual void slot24(int value);
    virtual void slot25(int value);
    virtual void slot26(int value);
    virtual void slot27(int value);
    virtual void slot28(int value);
    virtual void slot29(int value);
    virtual void slot30(int value);
    virtual void slot31(int value);
    virtual void slot32(int value);
    virtual void slot33(int value);
    virtual void slot34(int value);
    virtual void slot35(int value);
};

struct Self {
    char pad0[0x28];
    int speed;
    char pad2c[0x5c];
    unsigned short kind;
    char pad8a[0x2e];
    int countdown;
    int rate;
    Object *child;
};

extern "C" {
extern char data_020cfd44;
extern Object *func_02063b70(void *heap);
extern void func_020153d8(Object *kid, void *owner);
extern void func_0203575c(Object *kid, void *owner);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Object *object, int layer, int flags);
}

extern "C" void func_020472f0(Self *self)
{
    Object *child;
    int speed;
    child = func_02063b70(&data_020cfd44);
    self->child = child;
    child->slot35(0);
    func_020153d8(self->child, self);
    func_0203575c(self->child, self);
    if (self->kind == 0x8035) {
        self->child->slot33(0x490);
    }
    func_0202f7e8(func_02009900(), self->child, 1, 0);
    self->countdown = 0;
    speed = self->speed;
    if (speed <= 0) {
        self->rate = 0x800;
        return;
    }
    if (speed < 0x1000) {
        self->rate = 0x1f000 / speed;
        return;
    }
    self->rate = 0x1f000 / (speed >> 12);
}
