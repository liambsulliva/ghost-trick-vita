class Sprite {
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
    virtual void s22(int a, int b, int c);
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30(unsigned short a);
    virtual void s31();
    virtual void s32();
    virtual void s33(unsigned short a);
    virtual void s34();
    virtual void s35(signed char a);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int a, int b, int c, int d, int e, int f, int g);
};

struct Rect {
    short x;
    short y;
    short w;
    short h;
};

class Source {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(struct Rect *rect, void *area, int a, int b);
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char pad;
    int alpha;
};

struct Window {
    char pad00[0xd8];
    Sprite *back;
    Sprite *front;
    short id;
    char pade2[1];
    unsigned char ready;
    char area[0x34];
    int back_frame;
    int front_frame;
};

extern "C" {
extern short func_02047914(Source *source);
extern void func_0205765c(Sprite *p, int a, int b, int c, int d);
extern void func_0205700c(Sprite *p, int a, int b, int c, int d);
extern void func_0202e46c(struct Rect *rect);
extern int func_02047b38(Source *source);
extern void func_02047b3c(struct Color *color, int value);
extern void func_0200ee34(Sprite *p, unsigned char r, unsigned char g, unsigned char b);
int func_0204791c(struct Window *window, Source *source, int a, int b, unsigned short *base);
}

int func_0204791c(struct Window *window, Source *source, int a, int b, unsigned short *base)
{
    struct Rect rects[4];
    struct Color color;
    int dx;
    int dy;

    if (window->id != func_02047914(source)) {
        return 0;
    }
    if (window->ready == 0) {
        window->back->s42(0xbe, 0, 1, 0, 1, -1, 0);
        func_0205765c(window->back, 0, 0, 0x10000, 0x48000);
        func_0205700c(window->back, -0x8000, 0x40000, 0x8000, -0x10000);
        window->front->s42(0xbf, 0, 1, 0, 1, -1, 0);
        func_0205765c(window->front, 0, 0, 0x10000, 0x48000);
        func_0205700c(window->front, -0x8000, 0x40000, 0x8000, -0x10000);
        window->ready = 1;
    }
    func_0202e46c(&rects[0]);
    func_0202e46c(&rects[1]);
    func_0202e46c(&rects[2]);
    func_0202e46c(&rects[3]);
    source->s8(rects, window->area, a, b);
    dx = rects[2].x - rects[0].x;
    dy = rects[0].y - rects[2].y;
    func_02047b3c(&color, func_02047b38(source));
    window->back->s33((unsigned short)(*base - 2));
    window->back->s30((unsigned short)window->back_frame);
    window->back->s22(dx, dy, 0x1000);
    func_0200ee34(window->back, color.r, color.g, color.b);
    window->back->s35((signed char)((color.alpha * 31 / 100) >> 12));
    window->front->s33((unsigned short)(*base - 1));
    window->front->s30((unsigned short)window->front_frame);
    window->front->s22(dx, dy, 0x1000);
    func_0200ee34(window->front, color.r, color.g, color.b);
    window->front->s35((signed char)((color.alpha * 31 / 100) >> 12));
    return 0;
}
