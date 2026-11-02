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
    virtual void s14(int x);
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
    virtual void s35(signed char alpha);
};

struct Offsets {
    int v[6];
};

struct Panel {
    char pad0[0x11c];
    Label *grid[2][6];
    Label *extra[2];
};

extern "C" {
extern const struct Offsets data_ov003_020d5a5c;
void func_ov003_020d4ef4(struct Panel *panel, int style, int x, int y);
}

void func_ov003_020d4ef4(struct Panel *panel, int style, int x, int y)
{
    struct Offsets offsets;
    int i;

    offsets = data_ov003_020d5a5c;
    for (i = 0; i < 6; i++) {
        if (panel->grid[style][i] != 0) {
            panel->grid[style][i]->s13(x + (offsets.v[i] << 12), y + 0x8000, 0);
        }
    }
    if (panel->extra[style] != 0) {
        panel->extra[style]->s13(x, y + 0x20000, 0);
    }
}
