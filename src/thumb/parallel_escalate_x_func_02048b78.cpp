struct Item {
    unsigned char pad0;
    unsigned char active;
    unsigned short tilt;
    int x;
    int y;
    char pad0c[8];
    int scale;
    char pad18[4];
    int angle;
};

class Field {
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
    virtual void s33();
    virtual void s34();
    virtual void s35(signed char value);
    virtual void s36();
    virtual void s37(signed char value);
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();

    char pad4[0x3e];
    unsigned short base_y;
    signed char alpha;
    char pad45[0x73];
    Item *items;
};

extern "C" {
extern void *func_02008444(void);
extern int func_0202c394(void *object);
extern int func_0203435c(void *object);
extern int func_02010b54(Field *field);
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern int func_0200f5c8(void *object);
extern int func_0200f5e8(void *object);
extern void func_02048734(void);
extern void func_02048740(int first, int second, int third);
extern void func_02048d08(int x, int y, int z);
extern void func_020487b0(int value);
extern void *func_0200eb78(void);
extern int func_02022ca4(void *owner, int value);
extern void func_020487bc(int value);
extern void func_02048d18(int s, int t);
extern void func_020487c8(short x, short y, short z);
extern void func_020487e4(void);
void func_02048b78(Field *field);
}

void func_02048b78(Field *field)
{
    unsigned int i;
    int depth;
    int x;
    int y;

    if (func_0202c394(func_02008444()) != 0 || func_0203435c(func_02008444()) != 0 || func_02010b54(field) == 0) {
        return;
    }
    depth = -(func_02029708(func_0202970c()) - field->base_y);
    for (i = 0; i < 0x20; i++) {
        if (field->items[i].active == 0) {
            continue;
        }
        x = field->items[i].x - func_0200f5c8(func_02008444()) - 0x80000;
        y = -field->items[i].y + func_0200f5e8(func_02008444());
        func_02048734();
        func_02048740(x >> 7, ((y + 0x60000) / 3) >> 5, depth);
        func_02048d08(0x1000, 0x1555, 0x1000);
        func_02048d08(field->items[i].scale, field->items[i].scale, 0x1000);
        field->s35(field->items[i].angle >> 12);
        field->s37(field->items[i].tilt);
        if (field->alpha > 0) {
            field->s41();
            func_020487b0(1);
            func_020487bc(func_02022ca4(func_0200eb78(), 0x6318));
            func_02048d18(0, 0);
            func_020487c8(-0x400, 0x400, 0);
            func_02048d18(0, 0x40000);
            func_020487c8(-0x400, -0x400, 0);
            func_02048d18(0x40000, 0x40000);
            func_020487c8(0x400, -0x400, 0);
            func_02048d18(0x40000, 0);
            func_020487c8(0x400, 0x400, 0);
            func_020487e4();
        }
    }
}
