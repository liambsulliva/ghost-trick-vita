class Owner {
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
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44(void *layout);
};

struct Size {
    char pad[4];
    short width;
    short height;
    int color;
};

struct Res {
    char pad[8];
    unsigned char font;
    char pad9[3];
    int text;
    int pad10;
    int frame;
    int frame_size;
};

struct Panel {
    char pad[0xdc];
    void *layout;
    char pade0[0x28];
    unsigned short state;
    signed char size;
    char pad10b;
    short width;
    short scale;
    int color;
    Res *res;
    unsigned char ready;
};

extern "C" {
extern void *func_02031254(int size);
extern void *func_02042718(void *p);
extern void func_02042820(void *layout, char *text, int x, int a, int b, int c, int d, int e);
extern void func_020428f0(void *layout, char *frame, int size, int a, int b, int c, int d, int e, int f);
extern int func_02029020(void);
extern void func_02042a44(void *layout);
extern void func_02042664(void *layout);
extern void func_02029014(void *layout, int color);
extern void func_02029018(void *layout, int color);
extern void func_0202901c(void *layout, unsigned char font);
extern void func_020458a8(void *layout, short width);
extern void func_020458ac(void *layout, short height);
extern void func_0205700c(Owner *owner, int left, int top, int right, int bottom);
extern void func_0205765c(Owner *owner, int a, int b, int width, int height);
int func_020456e0(Panel *panel, Owner *owner, Size *sizes, int size);
}

int func_020456e0(Panel *panel, Owner *owner, Size *sizes, int size)
{
    Res *res;
    int x;
    int frame;
    char *text;
    int t;
    int w;
    int h;
    void *p;

    panel->state = 0;
    panel->size = size;
    res = panel->res;
    if (res == 0) {
        return 0;
    }
    frame = res->frame;
    text = (char *)res + res->text;
    x = (panel->width * sizes[panel->size].width) >> 1;
    if (panel->layout == 0) {
        p = func_02031254(0x54);
        if (p != 0) {
            p = func_02042718(p);
        }
        panel->layout = p;
        func_02042820(panel->layout, text + x, (panel->scale * panel->width) >> 1, 2, 1, 1, 0xffff, -1);
        func_020428f0(panel->layout, (char *)res + frame, res->frame_size, 2, 0, 0xffff, 0, -1, 0x100);
        panel->ready = 1;
    } else {
        t = func_02029020();
        func_02042a44(panel->layout);
        func_02042820(panel->layout, text + x, (panel->width * sizes[panel->size].height) >> 1, 2, 1, 1, 0xffff, t);
        func_02042664(panel->layout);
    }
    func_02029014(panel->layout, panel->color);
    func_02029018(panel->layout, sizes[panel->size].color);
    func_0202901c(panel->layout, res->font);
    func_020458a8(panel->layout, panel->width);
    func_020458ac(panel->layout, sizes[panel->size].height);
    owner->s44(panel->layout);
    w = panel->width << 12;
    h = sizes[panel->size].height << 12;
    func_0205700c(owner, -(w >> 1), h >> 1, w >> 1, -(h >> 1));
    func_0205765c(owner, 0, 0, w, h);
    return 1;
}
