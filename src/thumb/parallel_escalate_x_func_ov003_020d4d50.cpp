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

struct Panel {
    char pad0[0x24];
    Label *top;
    Label *bottom;
    Label *title;
    Label *text;
    char pad34[4];
    Label *cursor;
    Label *arrows[2];
    Label *left[18];
    Label *middle[18];
    Label *right[18];
    char pad11c[0x3c];
    Label *tabs[2];
    Label *marks[2];
    unsigned char page;
    char pad169[3];
    unsigned char style;
    char pad16d[7];
    unsigned int frame;
    char pad178[8];
    int rows;
};

extern "C" {
extern void func_ov003_020d4ef4(struct Panel *panel, int style, int x, int y);
extern void func_ov003_020d4c48(struct Panel *panel);
extern void func_ov003_020d4818(struct Panel *panel);
void func_ov003_020d4d50(struct Panel *panel, int t, int page);
}

void func_ov003_020d4d50(struct Panel *panel, int t, int page)
{
    int fade;
    int y;
    int x;
    int lift;
    int rest;
    int i;

    fade = 31 - t * 31 / 28;
    y = t * 28 / 28;
    x = t * 256 / 28;
    lift = t * 64 / 28;
    func_ov003_020d4ef4(panel, panel->style, -lift << 12, y << 12);
    y += 32;
    panel->top->s13(-0x80000, y << 12, 0);
    panel->bottom->s13(-0x80000, y << 12, 0);
    for (i = 0; i < panel->rows; i++) {
        if (panel->right[i] != 0) {
            panel->right[i]->s35(fade);
        }
        if (panel->left[i] != 0) {
            panel->left[i]->s35(fade);
        }
        if (panel->middle[i] != 0) {
            panel->middle[i]->s35(fade);
        }
    }
    rest = 31 - fade;
    panel->cursor->s35(rest);
    panel->bottom->s35(rest);
    for (i = 0; i < 2; i++) {
        panel->arrows[i]->s35(rest);
    }
    for (i = 0; i < 2; i++) {
        panel->tabs[i]->s35(fade);
    }
    panel->title->s35(fade);
    panel->text->s35(rest);
    for (i = 0; i < 2; i++) {
        if (panel->marks[i] != 0) {
            panel->marks[i]->s14((i * 0x58 + 0xd4 - x) << 12);
        }
    }
    func_ov003_020d4c48(panel);
    panel->frame++;
    if (panel->frame > 28) {
        panel->frame = 0;
        panel->page = page;
    }
    func_ov003_020d4818(panel);
}
