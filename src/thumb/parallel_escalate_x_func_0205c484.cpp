class Picker {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33(int a);
    char pad4[0xc1];
    unsigned char ready;
    unsigned char mode;
    char pad_c7;
    unsigned short (*rows)[8];
    unsigned short *colors;
    char blit[0x54];
};

extern "C" {
extern void func_02042b20(void *blit);
extern void func_02029014(void *blit, int value);
extern void func_02029018(void *blit, int value);
extern void func_0202901c(void *blit, int value);
extern void func_02042820(void *blit, void *dst, unsigned int size, unsigned int count, unsigned char mode, int flag, unsigned int id, int request);
extern void func_020428f0(void *blit, void *data, int size, int p3, int p4, unsigned short c, int d, int f, int g);
void func_0205c484(Picker *picker);
}

void func_0205c484(Picker *picker)
{
    int l;
    int d;
    unsigned int i;

    if (picker->ready != 0) {
        return;
    }
    func_02042b20(picker->blit);
    func_02029014(picker->blit, 0);
    func_02029018(picker->blit, 0);
    func_0202901c(picker->blit, 6);
    if (picker->mode == 1) {
        picker->colors[1] = 0xc03;
        picker->colors[2] = 0xfe3;
        l = 0x29;
        d = 0x2a;
    } else if (picker->mode == 2) {
        picker->colors[1] = 0x63;
        picker->colors[2] = 0x7c63;
        l = 0x29;
        d = 0x2a;
    } else {
        picker->colors[1] = 0xc60;
        picker->colors[2] = 0xc7f;
        l = 0x29;
        d = 0x2a;
    }
    l |= l << 8;
    d |= d << 8;
    for (i = 0; i < 4; i++) {
        picker->rows[i][0] = picker->rows[i][1] = picker->rows[i][2] = picker->rows[i][3] = (unsigned short)l;
        picker->rows[i][4] = picker->rows[i][5] = picker->rows[i][6] = picker->rows[i][7] = (unsigned short)d;
    }
    func_02042820(picker->blit, picker->rows, 0x40, 4, 1, 0, 0xffff, -1);
    func_020428f0(picker->blit, picker->colors, 0x10, 4, 0, 0xffff, 0, -1, 0x100);
    picker->s33(0x918);
    picker->ready = 1;
}
