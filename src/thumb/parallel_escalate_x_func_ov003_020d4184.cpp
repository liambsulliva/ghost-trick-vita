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
    virtual void s33(int depth);
    virtual void s34();
    virtual void s35(signed char alpha);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(unsigned short id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51(int on);
};

struct Table4 {
    int v[4];
};

struct Names {
    unsigned short v[18];
};

struct Vec {
    short x;
    short y;
    short z;
    short w;
};

struct Panel {
    char pad0[0x44];
    Label *left[18];
    Label *middle[18];
    Label *right[18];
    Label *grid[2][6];
    Label *extra[2];
    char pad154[0x18];
    unsigned char style;
    unsigned char wrap;
    unsigned char start;
    char pad16f[9];
    int cur;
    int target;
    int rows;
    char pad184[4];
    unsigned char list[0x13];
};

extern "C" {
extern char data_020cf724;
extern const struct Table4 data_ov003_020d5a3c;
extern const struct Table4 data_ov003_020d5a1c;
extern const struct Table4 data_ov003_020d5a2c;
extern const struct Table4 data_ov003_020d5a4c;
extern const struct Names data_ov003_020d5a74;
extern void *func_02008444(void);
extern signed char *func_0203cbe4(void *object);
extern int func_ov003_020d50b8(struct Panel *panel, int n);
extern void func_0202f64c(void *slot);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *object, unsigned int a, int b);
extern Label *func_0205693c(void *type);
extern void func_02046838(Label *object, int value);
extern void func_0200ee34(Label *object, unsigned int a, unsigned int b, unsigned int c);
extern void func_ov003_020d47fc(struct Vec *dst, struct Vec *src);
extern struct Vec *func_ov003_020d4810(Label *label, int index);
extern void func_0202c700(Label *label, unsigned int index, struct Vec *pos);
extern void func_ov003_020d4818(struct Panel *panel);
void func_ov003_020d4184(struct Panel *panel, int index);
}

void func_ov003_020d4184(struct Panel *panel, int index)
{
    struct Table4 x_offsets;
    struct Table4 y_offsets;
    struct Table4 x_extra;
    struct Table4 y_extra;
    struct Names names;
    struct Vec pos;
    struct Vec base;
    struct Vec corner;
    int value;
    signed char *text;
    int mult;
    unsigned int k;
    signed char digit;
    int hour;
    int minute;
    char pm;
    int i;
    int j;
    int m;
    int w;

    x_offsets = data_ov003_020d5a3c;
    y_offsets = data_ov003_020d5a1c;
    x_extra = data_ov003_020d5a2c;
    y_extra = data_ov003_020d5a4c;
    value = 0;
    if (index < 0) {
        if (panel->wrap != 0) {
            value = func_ov003_020d50b8(panel, panel->start + 1);
        } else {
            text = func_0203cbe4(func_02008444());
            mult = 1000;
            for (k = 0; k < 4; k++) {
                digit = text[k] - '0';
                if (digit < 0) {
                    digit = 0;
                }
                if (digit > 9) {
                    digit = 9;
                }
                value += digit * mult;
                mult /= 10;
            }
        }
    } else if (index < 0x12) {
        value = func_ov003_020d50b8(panel, panel->list[index] + 1);
    }
    hour = value / 100 % 24;
    minute = value % 100;
    pm = 0;
    if (hour >= 12) {
        hour -= 12;
        pm = 1;
    }
    if (hour == 0) {
        hour = 12;
    }
    panel->style = 1 - panel->style;
    for (i = 0; i < 6; i++) {
        if (i == 1 && hour < 10) {
            func_0202f64c(&panel->grid[panel->style][i]);
        } else if (panel->grid[panel->style][i] == 0) {
            panel->grid[panel->style][i] = func_0205693c(&data_020cf724);
            panel->grid[panel->style][i]->s51(1);
            panel->grid[panel->style][i]->s33(0x330);
            func_0202f7e8(func_02009900(), panel->grid[panel->style][i], 1, 0);
        }
    }
    if (panel->extra[panel->style] == 0) {
        panel->extra[panel->style] = func_0205693c(&data_020cf724);
        panel->extra[panel->style]->s51(1);
        panel->extra[panel->style]->s33(0x330);
        func_0202f7e8(func_02009900(), panel->extra[panel->style], 1, 0);
    }
    panel->grid[panel->style][0]->s42(0x49 + pm, 0, 1, 0, 1, -1, 0);
    if (panel->grid[panel->style][1] != 0) {
        panel->grid[panel->style][1]->s42(hour / 10 + 0x3e, 0, 1, 0, 1, -1, 0);
    }
    panel->grid[panel->style][2]->s42(hour % 10 + 0x3e, 0, 1, 0, 1, -1, 0);
    panel->grid[panel->style][3]->s42(0x48, 0, 1, 0, 1, -1, 0);
    panel->grid[panel->style][4]->s42(minute / 10 + 0x3e, 0, 1, 0, 1, -1, 0);
    panel->grid[panel->style][5]->s42(minute % 10 + 0x3e, 0, 1, 0, 1, -1, 0);
    panel->extra[panel->style]->s42(panel->list[panel->target] % 18 * 5 + 0x1c4, 0, 1, 0, 1, -1, 0);
    names = data_ov003_020d5a74;
    for (i = 0; i < panel->rows; i++) {
        if (i == panel->cur || i == panel->target || i == panel->cur - 1 || i == panel->target - 1 || i == panel->cur + 1 || i == panel->target + 1) {
            w = i * 4;
            if (panel->left[i] == 0) {
                panel->left[i] = func_0205693c(&data_020cf724);
                panel->left[i]->s51(1);
                panel->left[i]->s33(0x340);
                func_02046838(panel->left[i], 1);
                func_0202f7e8(func_02009900(), panel->left[i], 1, 0);
            }
            (*(Label **)((char *)panel + i * 4 + 0x44))->s42(names.v[panel->list[i] % 18], 1, 1, 0, 1, -1, 0);
            if ((*(Label **)((char *)panel + i * 4 + 0x8c)) == 0) {
                (*(Label **)((char *)panel + i * 4 + 0x8c)) = func_0205693c(&data_020cf724);
                (*(Label **)((char *)panel + i * 4 + 0x8c))->s51(1);
                (*(Label **)((char *)panel + i * 4 + 0x8c))->s33(0x350);
                if (i == panel->cur) {
                    func_0200ee34(panel->middle[i], 0x1f, 0, 0);
                } else {
                    func_0200ee34(panel->middle[i], 0x1c, 0xa, 0);
                }
                func_0202f7e8(func_02009900(), panel->middle[i], 1, 0);
            }
            for (j = 0; j < 4; j++) {
                func_ov003_020d47fc(&pos, func_ov003_020d4810(panel->left[i], j));
                pos.x += (short)x_offsets.v[j];
                pos.y += (short)y_offsets.v[j];
                func_0202c700(panel->middle[i], j, &pos);
            }
            if (panel->right[i] == 0) {
                (*(Label **)((char *)panel + i * 4 + 0xd4)) = func_0205693c(&data_020cf724);
                (*(Label **)((char *)panel + i * 4 + 0xd4))->s51(1);
                (*(Label **)((char *)panel + i * 4 + 0xd4))->s33(0x360);
                func_0200ee34((*(Label **)((char *)panel + i * 4 + 0xd4)), 0x1c, 0xa, 0);
                func_0202f7e8(func_02009900(), (*(Label **)((char *)panel + i * 4 + 0xd4)), 1, 0);
            }
            func_ov003_020d47fc(&base, func_ov003_020d4810(panel->left[i], 0));
            for (m = 0; m < 4; m++) {
                func_ov003_020d47fc(&corner, &base);
                corner.x += (short)x_extra.v[m];
                corner.y += (short)y_extra.v[m];
                func_0202c700(panel->right[i], m, &corner);
            }
            if (i == 0) {
                func_0202f64c((Label **)((char *)panel + 0xd4 + w));
            }
        } else {
            func_0202f64c(&panel->left[i]);
            func_0202f64c(&panel->middle[i]);
            func_0202f64c(&panel->right[i]);
        }
    }
    func_ov003_020d4818(panel);
}
