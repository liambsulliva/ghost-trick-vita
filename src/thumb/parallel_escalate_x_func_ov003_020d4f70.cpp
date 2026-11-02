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
    virtual void s33(int a);
    virtual void s34();
    virtual void s35(int a);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int a, int b, int c, int d, int e, int f, int g);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int a, int b);
};

struct Pair {
    unsigned short v[2];
};

struct Panel {
    char pad0[0x30];
    Label *text;
    char pad34[4];
    Label *cursor;
    char pad3c[0x124];
    Label *marks[2];
    char pad168;
    unsigned char wide;
};

extern "C" {
extern char data_020cf724[];
extern char data_020ced58[];
extern const struct Pair data_ov003_020d5a0c;
extern void *func_02009900(void);
extern Label *func_0205693c(char *a);
extern Label *func_0204edb4(char *a);
extern void func_0200ee34(Label *a, int b, int c, int d);
extern void func_0202f7e8(void *a, void *b, int c, int d);
extern void func_0203cdd8(Label *a, unsigned short b, unsigned short c, unsigned short d);
extern void func_0203bfb4(Label *a, unsigned char b);
void func_ov003_020d4f70(struct Panel *panel);
}

void func_ov003_020d4f70(struct Panel *panel)
{
    struct Pair ids;
    int i;

    if (panel->cursor == 0) {
        (panel->cursor = func_0205693c(data_020cf724))->s46(0x100000, 0x90000);
        panel->cursor->s33(0x380);
        panel->cursor->s13(0, 0, 0);
        func_0200ee34(panel->cursor, 0, 0, 0);
        panel->cursor->s35(0);
        func_0202f7e8(func_02009900(), panel->cursor, 1, 0);
    }
    if (panel->wide) {
        panel->text->s42(0x1bf, 0, 1, 0, 1, -1, 0);
    } else {
        panel->text->s42(0x1ba, 0, 1, 0, 1, -1, 0);
    }
    ids = data_ov003_020d5a0c;
    for (i = 0; i < 2; i++) {
        if (panel->marks[i] == 0) {
            panel->marks[i] = func_0204edb4(data_020ced58);
            func_0203cdd8(panel->marks[i], ids.v[i], ids.v[i] + 5, ids.v[i] + 10);
            func_0203bfb4(panel->marks[i], 1 - i);
            panel->marks[i]->s33(0x330);
            panel->marks[i]->s13((i * 0x58 + 0xd4) << 12, -0x28000, 0);
            func_0202f7e8(func_02009900(), panel->marks[i], 1, 0);
        }
    }
}
