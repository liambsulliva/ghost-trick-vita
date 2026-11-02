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
};

struct Owner {
    char pad0[0xd90];
    short scale;
    short pad_d92;
    int x;
    int y;
    char pad_d9c[0x2c];
    short width;
    short height;
    int offset_x;
    int offset_y;
    int palette;
    char pad_dd8[0x108];
    void *engine;
    int mode;
    int pad_ee8;
    Label *top;
    Label *bottom;
};

extern "C" {
extern char data_020cf93c[];
extern char data_020cf724[];
extern void *func_02009900(void);
extern void func_0202fad4(void *a, char *b);
extern Label *func_0205693c(char *a);
extern void func_0200ee34(Label *a, int b, int c, int d);
extern void func_0202f7e8(void *a, void *b, int c, int d);
extern void func_0200ee54(Owner *owner, int value);
extern void *func_02031254(int size);
extern void *func_02009858(void *object);
extern void func_02009a28(void *engine, int value);
extern int func_0200ee64(int value);
extern void func_02009c40(void *engine, unsigned short a, unsigned short b);
extern int func_0200ee68(void *engine);
extern int func_0200ee74(void *engine);
extern void func_0200a2f8(void *engine, int value);
extern void func_0200a39c(void *engine, int value, int arg);
void func_0200ecb0(Owner *owner, int scale, int arg);
}

void func_0200ecb0(Owner *owner, int scale, int arg)
{
    void *engine;

    func_0202fad4(func_02009900(), data_020cf93c);
    if (owner->top == 0) {
        (owner->top = func_0205693c(data_020cf724))->s33(0xc00);
        func_0200ee34(owner->top, 0, 0, 0);
        owner->top->s11();
        func_0202f7e8(func_02009900(), owner->top, 1, 0);
    }
    if (owner->bottom == 0) {
        (owner->bottom = func_0205693c(data_020cf724))->s33(0xc00);
        func_0200ee34(owner->bottom, 0, 0, 0);
        owner->bottom->s11();
        func_0202f7e8(func_02009900(), owner->bottom, 1, 0);
    }
    func_0200ee54(owner, 0);
    if (owner->engine == 0) {
        engine = func_02031254(0xaa48);
        if (engine != 0) {
            engine = func_02009858(engine);
        }
        owner->engine = engine;
    }
    if (owner->engine != 0) {
        owner->scale = scale;
        if (owner->x < 0) {
            owner->x = 0;
        }
        if (owner->y < 0) {
            owner->y = 0;
        }
        func_02009a28(owner->engine, scale);
        func_02009c40(owner->engine, func_0200ee64(owner->x), func_0200ee64(owner->y));
        owner->offset_x = (func_0200ee68(owner->engine) - 0x100) << 12;
        owner->offset_y = (func_0200ee74(owner->engine) - 0xc0) << 12;
        owner->width = func_0200ee68(owner->engine);
        owner->height = func_0200ee74(owner->engine);
        func_0200a2f8(owner->engine, owner->palette);
        func_0200a39c(owner->engine, owner->mode, arg);
    }
}
