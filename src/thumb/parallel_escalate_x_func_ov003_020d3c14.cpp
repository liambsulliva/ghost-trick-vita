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
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int w, int h);
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51(int on);
};

struct Config {
    char pad0[5];
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    unsigned int mask;
    char pad0c;
    unsigned char bd;
};

struct Panel {
    char pad0[8];
    struct Config *source;
    char pad0c[0x14];
    void *sprite;
    Label *top;
    Label *bottom;
    Label *title;
    Label *text;
    Label *frame;
    Label *cursor;
    Label *arrows[2];
    Label *left[18];
    Label *middle[18];
    Label *right[18];
    Label *grid[2][6];
    Label *extra[2];
    int tab;
    Label *tabs[2];
    Label *marks[2];
    unsigned char page;
    char pad169[4];
    unsigned char wrap;
    unsigned char start;
    unsigned char count;
    unsigned char kind;
    char pad171[0xf];
    int rows;
    struct Config *config;
    unsigned char list[0x13];
};

extern "C" {
extern char data_020cb04c;
extern char data_020cf724;
extern char data_020ced58;
extern char data_ov003_020d5ae0;
extern signed char data_ov003_020d5b40;
extern void *func_02008444(void);
extern void func_0202663c(void *a, struct Config *config);
extern unsigned int func_ov003_020d4154(unsigned int x);
extern int func_ov003_020d4138(unsigned int x);
extern int func_ov003_020d4168(void *dst, int size);
extern void *func_0202fe20(void *a);
extern void func_ov003_020d4174(void *a);
extern void func_ov003_020d417c(void *a, unsigned int b);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *object, unsigned int a, int b);
extern Label *func_0205693c(void *type);
extern void func_02046838(Label *object, int value);
extern void func_0200ee34(Label *object, unsigned int a, unsigned int b, unsigned int c);
extern void func_0205765c(Label *object, int a, int b, int c, int d);
extern Label *func_0204edb4(void *type);
extern void func_0203cdd8(Label *object, unsigned short a, unsigned short b, unsigned short c);
extern void func_0203bfb4(Label *object, int value);
extern void func_020521d4(void *object, int a, unsigned int b, int c);
void func_ov003_020d3c14(struct Panel *panel);
}

void func_ov003_020d3c14(struct Panel *panel)
{
    struct Config *config;
    unsigned int mask;
    int count;
    int found;
    int i;
    int j;

    panel->config = panel->source;
    func_0202663c(func_02008444(), panel->config);
    config = panel->config;
    panel->start = config->b5 - 1;
    panel->wrap = config->b6;
    panel->count = config->b7;
    panel->kind = config->bd;
    mask = config->mask;
    panel->count = func_ov003_020d4138(func_ov003_020d4154(mask)) + 1;
    if (panel->wrap != 0) {
        panel->start++;
    }
    if (panel->start >= 0x13) {
        panel->start = 0x12;
    }
    count = panel->count;
    func_ov003_020d4168(panel->list, 0x13);
    found = 0;
    panel->rows = 0;
    for (j = 0; j < count && j < 0x12; j++) {
        if ((mask & (1 << (j + 1))) != 0 || j == panel->start || j == 0) {
            panel->list[panel->rows] = j;
            panel->rows++;
            if (j + 1 == count) {
                found = 1;
            }
        }
    }
    if (found == 0 && panel->rows < 0x11 && count < 0x12) {
        panel->list[panel->rows] = count - 1;
        panel->rows++;
    }
    if (panel->rows == 0) {
        panel->rows = 1;
    }
    if (panel->rows > 0x12) {
        panel->rows = 0x12;
    }
    for (i = 0; i < 2; i++) {
        panel->extra[i] = 0;
        for (j = 0; j < 6; j++) {
            panel->grid[i][j] = 0;
        }
    }
    for (i = 0; i < 18; i++) {
        panel->left[i] = 0;
        panel->middle[i] = 0;
        panel->right[i] = 0;
    }
    panel->sprite = func_0202fe20(&data_020cb04c);
    func_ov003_020d4174(panel->sprite);
    func_ov003_020d417c(panel->sprite, 0);
    func_0202f7e8(func_02009900(), panel->sprite, 1, 0);
    panel->top = func_0205693c(&data_020cf724);
    panel->top->s51(1);
    panel->top->s42(0x4c, 0, 1, 0, 1, -1, 0);
    panel->top->s13(-0x80000, 0x20000, 0);
    panel->top->s33(0x370);
    func_02046838(panel->top, 1);
    func_0202f7e8(func_02009900(), panel->top, 1, 0);
    panel->bottom = func_0205693c(&data_020cf724);
    panel->bottom->s51(1);
    panel->bottom->s42(0x4d, 0, 1, 0, 1, -1, 0);
    panel->bottom->s13(-0x80000, 0x20000, 0);
    panel->bottom->s33(0x360);
    func_02046838(panel->bottom, 1);
    panel->bottom->s35(0);
    func_0202f7e8(func_02009900(), panel->bottom, 1, 0);
    panel->title = func_0205693c(&data_020cf724);
    panel->title->s42(0x1b5, 0, 1, 0, 1, -1, 0);
    panel->title->s33(0x330);
    panel->title->s13(0, -0x54000, 0);
    func_02046838(panel->title, 1);
    func_0202f7e8(func_02009900(), panel->title, 1, 0);
    panel->text = func_0205693c(&data_020cf724);
    panel->text->s42(0x1ba, 0, 1, 0, 1, -1, 0);
    panel->text->s35(0);
    panel->text->s13(0, -0xc000, 0);
    panel->text->s33(0x330);
    func_02046838(panel->text, 1);
    func_0202f7e8(func_02009900(), panel->text, 1, 0);
    panel->frame = func_0205693c(&data_020cf724);
    panel->frame->s46(0x100000, 0xc0000);
    func_0200ee34(panel->frame, 0, 0, 0);
    panel->frame->s35(data_ov003_020d5b40);
    panel->frame->s33(0x390);
    func_0202f7e8(func_02009900(), panel->frame, 1, 0);
    panel->cursor = 0;
    for (i = 0; i < 2; i++) {
        panel->arrows[i] = func_0205693c(&data_020cf724);
        panel->arrows[i]->s42(0x12, 0, 1, 1, 1, -1, 0);
        panel->arrows[i]->s46(0x110000, 0x10000);
        func_0205765c(panel->arrows[i], 0, 0, 0x110000, 0x10000);
        panel->arrows[i]->s13(0x8000, (0x40 - i * 0x80) << 12, 0);
        panel->arrows[i]->s33(0x330);
        panel->arrows[i]->s35(0);
        func_0202f7e8(func_02009900(), panel->arrows[i], 1, 0);
    }
    panel->tab = 0;
    for (i = 0; i < 2; i++) {
        panel->tabs[i] = func_0204edb4(&data_020ced58);
        func_0203cdd8(panel->tabs[i], i * 3 + 0x27, i * 3 + 0x29, i * 3 + 0x28);
        panel->tabs[i]->s33(0x330);
        panel->tabs[i]->s13(0x70000, (0x50 - i * 0x88) << 12, 0);
        func_0202f7e8(func_02009900(), panel->tabs[i], 1, 0);
    }
    func_0203bfb4(panel->tabs[0], 1);
    if (panel->rows == 1) {
        func_0203bfb4(panel->tabs[1], 1);
    } else {
        func_0203bfb4(panel->tabs[1], 0);
    }
    for (i = 0; i < 2; i++) {
        panel->marks[i] = 0;
    }
    func_020521d4(&data_ov003_020d5ae0, 0, 0xf, 0x100);
    func_020521d4(&data_ov003_020d5ae0, 1, 0xf, 0x100);
}
