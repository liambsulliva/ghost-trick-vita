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
    virtual void slot36(int value);
    virtual void slot37(int value);
    virtual void slot38(int value);
    virtual void slot39(int value);
    virtual void slot40(int value);
    virtual void slot41(int value);
    virtual void slot42(int value);
    virtual void slot43(int value);
    virtual void slot44(int value);
    virtual void slot45(int value);
    virtual void slot46(int value);
    virtual void slot47(int value);
    virtual void slot48(int value);
    virtual void slot49(int value);
    virtual void slot50(int value);
    virtual void slot51(int value);
    virtual void slot52(int value);
    virtual void slot53(int value);
    virtual void slot54(int value);
    virtual void slot55(int value);
    virtual void slot56(int value);
    virtual void slot57(int value);
    virtual void slot58(int value);
    virtual void slot59(int value);
    virtual void slot60(int value);
    virtual void slot61(int value);
    virtual void slot62(int value);
    virtual void slot63(int value);
    virtual void slot64(int value);
    virtual void slot65(int value);
};

extern "C" {
extern char data_020cf7c8;
extern char data_020cfed4;
extern void *func_02008444(void);
extern int func_02031af4(void *system, int id);
extern void func_02031b04(void *system, int *id);
extern void func_0202f64c(Object **object);
extern void *func_02009900(void);
extern void func_0202fad4(void *system, void *data);
extern Object *func_02065b58(void *heap);
extern int func_02031b44(void *system);
extern void func_020153d8(Object *kid, void *owner);
extern void func_0202f7e8(void *system, Object *object, int layer, int flags);
}

extern "C" void func_02046e30(char *self, int arg)
{
    int id;
    id = *(int *)(self + 0x2c);
    if (id >= 0 && func_02031af4(func_02008444(), id) == 0 && *(Object **)(self + 0x28) != 0) {
        func_02031b04(func_02008444(), (int *)(self + 0x2c));
        func_0202f64c((Object **)(self + 0x28));
    }
    func_0202fad4(func_02009900(), &data_020cf7c8);
    *(Object **)(self + 0x28) = func_02065b58(&data_020cfed4);
    if (*(Object **)(self + 0x28) != 0) {
        *(int *)(self + 0x2c) = func_02031b44(func_02008444());
        (*(Object **)(self + 0x28))->slot65(arg);
        (*(Object **)(self + 0x28))->slot60(*(int *)(self + 0x2c));
        func_020153d8(*(Object **)(self + 0x28), self);
        func_0202f7e8(func_02009900(), *(Object **)(self + 0x28), 7, 0);
    }
}
