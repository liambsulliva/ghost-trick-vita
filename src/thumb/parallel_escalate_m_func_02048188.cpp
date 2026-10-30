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
};

extern "C" {
extern char data_020ce954;
extern void *func_02008444(void);
extern signed char *func_0203cbe4(void *system);
extern Object *func_02046f90(void *heap);
extern void func_0204821c(Object *object, int value);
extern void func_020153d8(Object *kid, void *owner);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Object *object, int layer, int flags);
}

extern "C" void func_02048188(char *self)
{
    unsigned int i;
    signed char *text;
    int scale;
    signed char digit;
    Object *child;
    i = 0;
    *(int *)(self + 0xc4) = 0;
    text = func_0203cbe4(func_02008444());
    scale = 1000;
    do {
        digit = text[i] - '0';
        if (digit < 0) {
            digit = 0;
        }
        if (digit > 9) {
            digit = 9;
        }
        *(int *)(self + 0xc4) += digit * scale;
        scale = (unsigned int)scale / 10;
        i++;
    } while (i < 4);
    child = func_02046f90(&data_020ce954);
    *(Object **)(self + 0xb8) = child;
    child->slot33(0x490);
    func_0204821c(*(Object **)(self + 0xb8), *(int *)(self + 0xc4));
    func_020153d8(*(Object **)(self + 0xb8), self);
    func_0202f7e8(func_02009900(), *(Object **)(self + 0xb8), 7, 0);
}
