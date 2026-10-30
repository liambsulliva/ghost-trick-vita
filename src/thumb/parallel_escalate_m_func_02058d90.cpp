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
};

struct Bits {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char mode : 2;
};

extern "C" {
extern void func_020591a4(...);
extern int func_02008444(...);
extern int func_0200f330(...);
extern void func_0200b598(...);
extern void func_0200bb8c(...);
extern void func_0200b7c0(...);
}

extern "C" void func_02058d90(char *param_1)
{
    short value;

    if (*(signed char *)(param_1 + 0xbf) != -1) {
        func_020591a4(param_1,*(signed char *)(param_1 + 0xbf),((struct Bits *)(param_1 + 0xde))->mode);
        *(signed char *)(param_1 + 0xbf) = -1;
    }
    if (func_0200f330(func_02008444()) == 0) {
        value = *(short *)(param_1 + 0xbc);
        if (value != -1) {
            if (((struct Bits *)(param_1 + 0xde))->bit4 == 0) {
                func_0200b598(param_1 + 0x28,value,0xf);
                ((Object *)param_1)->slot10();
            }
            else {
                func_0200b598(param_1 + 0x28,value,0xc);
                ((Object *)param_1)->slot11();
            }
            *(short *)(param_1 + 0xbc) = -1;
            if (*(unsigned short *)(param_1 + 0xc0) != 0) {
                func_0200bb8c(param_1 + 0x28,*(unsigned short *)(param_1 + 0xc0));
                *(unsigned short *)(param_1 + 0xc0) = 0;
            }
        }
        if (((struct Bits *)(param_1 + 0xde))->bit3 == 0) {
            func_0200b7c0(param_1 + 0x28);
        }
    }
}
