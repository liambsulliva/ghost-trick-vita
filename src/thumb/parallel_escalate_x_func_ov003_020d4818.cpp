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
    virtual int s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22(int x, int y, int z);
};

struct Panel {
    char pad0[0x3c];
    Label *arrows[2];
    Label *left[18];
    char pad8c[0x168 - 0x8c];
    unsigned char page;
    char pad169[3];
    unsigned char style;
    char pad16d[7];
    unsigned int frame;
    int cur;
    int target;
    int rows;
};

extern "C" {
extern void func_ov003_020d4ef4(struct Panel *panel, unsigned char style, int x, int y);
extern void func_ov003_020d4c48(struct Panel *panel);
extern void func_020436b8(Label *object, int value);
void func_ov003_020d4818(struct Panel *panel);
}

void func_ov003_020d4818(struct Panel *panel)
{
    int i;
    int angle;
    int turn;

    for (i = 0; i < 2; i++) {
        if (panel->arrows[i] != 0) {
            angle = panel->arrows[i]->s18();
            turn = angle - (panel->page == 3 ? 0x1400 : 0x400);
            if (turn <= -0x8000) {
                turn += 0x10000;
            }
            panel->arrows[i]->s14(turn);
        }
    }
    if (panel->page == 1 || panel->page == 2 || !panel->page) {
        if (panel->cur == panel->target) {
            func_ov003_020d4ef4(panel, panel->style, 0, 0);
            panel->left[panel->cur]->s13(-0x68000, 0x30000, 0);
            panel->left[panel->cur]->s22(0x1000, 0x1000, 0x1000);
            func_020436b8(panel->left[panel->cur], 0);
            if (panel->cur > 0) {
                (*(panel->left + panel->cur - 1))->s13(-0x68000, 0x80000, 0);
                (*(panel->left + panel->cur - 1))->s22(0xccd, 0xccd, 0x1000);
                func_020436b8(*(panel->left + panel->cur - 1), 1);
            }
            if (panel->cur < panel->rows - 1) {
                (*(panel->left + panel->cur + 1))->s13(-0x68000, -0x30000, 0);
                (*(panel->left + panel->cur + 1))->s22(0xccd, 0xccd, 0x1000);
                func_020436b8(*(panel->left + panel->cur + 1), 1);
            }
            func_ov003_020d4c48(panel);
            return;
        }
        func_ov003_020d4ef4(panel, 1 - panel->style, panel->frame * 32 * (panel->cur - panel->target) << 12, 0);
        func_ov003_020d4ef4(panel, panel->style, ((panel->target - panel->cur) * 256 + panel->frame * 32 * (panel->cur - panel->target)) << 12, 0);
        if (panel->target > panel->cur) {
            panel->left[panel->cur]->s13(-0x68000, panel->frame * 0x50000 / 8 + 0x30000, 0);
            func_020436b8(panel->left[panel->cur], 1);
            panel->left[panel->target]->s13(-0x68000, 0x30000 - (8 - panel->frame) * 0x60000 / 8, 0);
            func_020436b8(panel->left[panel->target], 0);
            if (panel->cur > 0) {
                (*(panel->left + panel->cur - 1))->s13(-0x68000, panel->frame * 0x50000 / 8 + 0x80000, 0);
                (*(panel->left + panel->cur - 1))->s22(0xccd, 0xccd, 0x1000);
                func_020436b8(*(panel->left + panel->cur - 1), 1);
            }
            if (panel->target < panel->rows - 1) {
                (*(panel->left + panel->target + 1))->s13(-0x68000, -0x30000 - (8 - panel->frame) * 0x50000 / 8, 0);
                (*(panel->left + panel->target + 1))->s22(0xccd, 0xccd, 0x1000);
                func_020436b8(*(panel->left + panel->target + 1), 1);
            }
        } else {
            panel->left[panel->cur]->s13(-0x68000, 0x30000 - panel->frame * 0x60000 / 8, 0);
            func_020436b8(panel->left[panel->cur], 1);
            panel->left[panel->target]->s13(-0x68000, (8 - panel->frame) * 0x50000 / 8 + 0x30000, 0);
            func_020436b8(panel->left[panel->target], 0);
            if (panel->cur < panel->rows - 1) {
                (*(panel->left + panel->cur + 1))->s13(-0x68000, -0x30000 - panel->frame * 0x50000 / 8, 0);
                (*(panel->left + panel->cur + 1))->s22(0xccd, 0xccd, 0x1000);
                func_020436b8(*(panel->left + panel->cur + 1), 1);
            }
            if (panel->target > 0) {
                (*(panel->left + panel->target - 1))->s13(-0x68000, (8 - panel->frame) * 0x50000 / 8 + 0x80000, 0);
                (*(panel->left + panel->target - 1))->s22(0xccd, 0xccd, 0x1000);
                func_020436b8(*(panel->left + panel->target - 1), 1);
            }
        }
        panel->left[panel->cur]->s22(0x1000 - panel->frame * 0x66, 0x1000 - panel->frame * 0x66, 0x1000);
        panel->left[panel->target]->s22(0x1000 - (8 - panel->frame) * 0x66, 0x1000 - (8 - panel->frame) * 0x66, 0x1000);
        func_ov003_020d4c48(panel);
    }
}
