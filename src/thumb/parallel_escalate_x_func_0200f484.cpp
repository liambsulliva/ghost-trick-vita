class Label {
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
    virtual void s13(int x, int y, int z);
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
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int width, int height);
};

struct Owner {
    char pad0[0xdbc];
    int base_x;
    int base_y;
    char pad_dc4[0x128];
    Label *top;
    Label *bottom;
};

extern "C" void func_0200f484(Owner *owner);

void func_0200f484(Owner *owner)
{
    short v;
    short h;
    short size;

    if (owner->bottom != 0) {
        if (owner->base_x != 0) {
            owner->bottom->s10();
            v = owner->base_x >> 12;
            if (v < 0) {
                v = -v;
            }
            h = v >> 1;
            size = (v + 1) & ~1;
            owner->bottom->s46(size << 12, 0xc0000);
            if (owner->base_x > 0) {
                owner->bottom->s13((0x80 - h) << 12, 0, 0);
            } else {
                owner->bottom->s13((h - 0x80) << 12, 0, 0);
            }
        } else {
            owner->bottom->s11();
        }
    }
    if (owner->top != 0) {
        if (owner->base_y != 0) {
            owner->top->s10();
            v = owner->base_y >> 12;
            if (v < 0) {
                v = -v;
            }
            h = v >> 1;
            size = (v + 1) & ~1;
            owner->top->s46(0x100000, size << 12);
            if (owner->base_y > 0) {
                owner->top->s13(0, (h - 0x60) << 12, 0);
            } else {
                owner->top->s13(0, (0x60 - h) << 12, 0);
            }
        } else {
            owner->top->s11();
        }
    }
}
