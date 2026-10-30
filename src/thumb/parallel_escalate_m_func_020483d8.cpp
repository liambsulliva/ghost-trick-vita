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
};

extern "C" {
extern int func_02048478(int value);
extern int func_02048488(int value);
extern void func_020484fc(void *self, unsigned char *entry, int flag);
}

extern "C" void func_020483d8(char *self)
{
    unsigned int count;
    unsigned int i;
    count = 0x80;
    *(unsigned char *)(self + 0xc4) = *(unsigned short *)(self + 0x88) - 0x8000;
    if (*(unsigned char *)(self + 0xc4) != 0) {
        count = 0x40;
    }
    for (i = 0; i < count; i++) {
        *(unsigned char *)(*(unsigned char **)(self + 0xb8) + i * 0x14 + 1) = (i & 0xf) + 1;
        *(unsigned char *)(*(unsigned char **)(self + 0xb8) + i * 0x14) = 0;
    }
    *(int *)(self + 0xbc) = func_02048478(0);
    *(int *)(self + 0xc0) = func_02048488(0);
    for (i = 0; i < count; i++) {
        func_020484fc(self, *(unsigned char **)(self + 0xb8) + i * 0x14, 1);
    }
    ((Object *)self)->slot33(0xa00);
    ((Object *)self)->slot37(0x37);
}
